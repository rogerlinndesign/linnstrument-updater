#if defined(__linux__)

#include "LinnStrumentSerialLinux.h"

#include "UpdaterApplication.h"

#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <iostream>
#include <limits.h>
#include <string>
#include <sys/stat.h>
#include <termios.h>
#include <unistd.h>

namespace
{
    constexpr const char* LinnStrumentVendorId  = "f055";
    constexpr const char* LinnStrumentProductId = "0070";

    std::string readSysfsFile(const std::string& path)
    {
        int fd = ::open(path.c_str(), O_RDONLY | O_CLOEXEC);

        if (fd < 0)
            return {};

        char buffer[256] = {};
        const ssize_t length = ::read(fd, buffer, sizeof(buffer) - 1);
        ::close(fd);

        if (length <= 0)
            return {};

        buffer[length] = '\0';

        std::string result(buffer);

        while (!result.empty() &&
               (result.back() == '\n' ||
                result.back() == '\r' ||
                result.back() == ' ' ||
                result.back() == '\t'))
        {
            result.pop_back();
        }

        return result;
    }

    bool isLinnStrumentSysfsDevice(const std::string& ttyPath)
    {
        char resolvedPath[PATH_MAX] = {};

        if (::realpath(ttyPath.c_str(), resolvedPath) == nullptr)
            return false;

        std::string path(resolvedPath);

        /*
            Walk upward through the sysfs hierarchy. The USB attributes are
            normally located above:

                .../1-2/1-2:1.0/tty/ttyACM0
        */
        while (!path.empty() && path != "/")
        {
            const std::string vendor =
                readSysfsFile(path + "/idVendor");

            const std::string product =
                readSysfsFile(path + "/idProduct");

            if (vendor == LinnStrumentVendorId &&
                product == LinnStrumentProductId)
            {
                return true;
            }

            const std::string manufacturer =
                readSysfsFile(path + "/manufacturer");

            const std::string deviceProduct =
                readSysfsFile(path + "/product");

            if (manufacturer == "Roger Linn Design" &&
                deviceProduct == "LinnStrument SERIAL")
            {
                return true;
            }

            const std::size_t slash = path.find_last_of('/');

            if (slash == std::string::npos)
                break;

            path.resize(slash);

            if (path.empty())
                path = "/";
        }

        return false;
    }

    bool configureSerialPort(int fd)
    {
        struct termios options {};

        if (::tcgetattr(fd, &options) < 0)
            return false;

        ::cfmakeraw(&options);

        if (::cfsetispeed(&options, B1200) < 0 ||
            ::cfsetospeed(&options, B1200) < 0)
        {
            return false;
        }

        options.c_cflag &= ~CSIZE;
        options.c_cflag |= CS8;
        options.c_cflag &= ~PARENB;
        options.c_cflag &= ~CSTOPB;
        options.c_cflag |= CLOCAL | CREAD;

        options.c_cc[VMIN]  = 0;
        options.c_cc[VTIME] = 0;

        if (::tcflush(fd, TCIFLUSH) < 0)
            return false;

        return ::tcsetattr(fd, TCSANOW, &options) == 0;
    }
}

LinnStrumentSerialLinux::LinnStrumentSerialLinux()
    : upgradeVerificationPhase(false),
      upgradeSuccessful(false)
{
}

LinnStrumentSerialLinux::~LinnStrumentSerialLinux()
{
}

String LinnStrumentSerialLinux::getFullLinnStrumentDevice()
{
    if (!isDetected())
        return String();

    return "/dev/" + linnstrumentDevice;
}

bool LinnStrumentSerialLinux::detect()
{
    std::cout << "Detecting device" << std::endl;

    resetDetection();

    DIR* directory = ::opendir("/sys/class/tty");

    if (directory == nullptr)
    {
        std::cerr << "Unable to open /sys/class/tty: "
                  << std::strerror(errno) << std::endl;
        return false;
    }

    struct dirent* entry = nullptr;

    while ((entry = ::readdir(directory)) != nullptr && !isDetected())
    {
        const char* name = entry->d_name;

        if (std::strcmp(name, ".") == 0 ||
            std::strcmp(name, "..") == 0)
        {
            continue;
        }

        /*
            LinnStrument normally appears as ttyACM*, but ttyUSB* is also
            accepted for compatibility with USB-to-serial adapters.
        */
        if (std::strncmp(name, "ttyACM", 6) != 0 &&
            std::strncmp(name, "ttyUSB", 6) != 0)
        {
            continue;
        }

        const std::string sysfsTtyPath =
            std::string("/sys/class/tty/") + name;

        if (!isLinnStrumentSysfsDevice(sysfsTtyPath))
            continue;

        linnstrumentDevice = String(name);

        std::cout << "Using LinnStrument device "
                  << linnstrumentDevice << std::endl;
    }

    ::closedir(directory);

    return isDetected();
}

bool LinnStrumentSerialLinux::isDetected()
{
    return linnstrumentDevice.isNotEmpty();
}

bool LinnStrumentSerialLinux::prepareDevice()
{
    if (!hasFirmwareFile() || !isDetected())
        return false;

    std::cout << "Preparing device" << std::endl;

    const String devicePath = getFullLinnStrumentDevice();

    const int fd = ::open(devicePath.toRawUTF8(),
                          O_RDWR | O_NOCTTY | O_CLOEXEC);

    if (fd < 0)
    {
        std::cerr << "Can't open serial device "
                  << devicePath << " ("
                  << std::strerror(errno) << ")"
                  << std::endl;
        return false;
    }

    const bool configured = configureSerialPort(fd);

    if (!configured)
    {
        std::cerr << "Unable to configure serial device "
                  << devicePath << " ("
                  << std::strerror(errno) << ")"
                  << std::endl;
    }

    ::close(fd);

    return configured;
}

bool LinnStrumentSerialLinux::performUpgrade()
{
    if (!hasFirmwareFile() || !isDetected())
        return false;

    std::cout << "Performing upgrade" << std::endl;

    upgradeOutput.clear();
    upgradeVerificationPhase = false;
    upgradeSuccessful = false;

    File currentApplication =
        File::getSpecialLocation(
            File::SpecialLocationType::currentExecutableFile);

    File applicationDirectory =
        currentApplication.getParentDirectory();

    File bossacTool =
        applicationDirectory
            .getChildFile("tools")
            .getChildFile("bossac");

    if (!bossacTool.isSymbolicLink()
        && !bossacTool.existsAsFile())
    {
        std::cerr << "Unable to find bossac at "
                  << bossacTool.getFullPathName()
                  << std::endl;
        return false;
    }
    StringArray args;

    args.add(bossacTool.getFullPathName());
    args.add("-i");
    args.add("--port=" + linnstrumentDevice);
    args.add("-U");
    args.add("false");
    args.add("-e");
    args.add("-w");
    args.add("-v");
    args.add("-b");
    args.add(firmwareFile);
    args.add("-R");

    if (!upgradeChild.start(args))
    {
        std::cerr << "Unable to start bossac" << std::endl;
        return false;
    }

    startTimer(1);

    return true;
}

void LinnStrumentSerialLinux::timerCallback()
{
    if (!upgradeChild.isRunning())
    {
        stopTimer();

        if (upgradeSuccessful)
            UpdaterApplication::getApp().restoreSettings();
        else
            UpdaterApplication::getApp().setUpgradeFailed();

        return;
    }

    char buffer = 0;

    if (upgradeChild.readProcessOutput(&buffer, 1) <= 0)
        return;

    upgradeOutput.append(String::charToString(buffer), 1);

    if (buffer != '%')
        return;

    if (upgradeOutput.contains("Verify"))
        upgradeVerificationPhase = true;

    const int index = upgradeOutput.lastIndexOf("] ");

    String progress;

    if (index >= 0)
        progress = upgradeOutput.substring(index + 2);
    else
        progress = upgradeOutput;

    progress = progress.trim();

    if (upgradeVerificationPhase && progress == "100%")
        upgradeSuccessful = true;

    UpdaterApplication::getApp().setProgressText(
        (upgradeVerificationPhase ? "Verifying... "
                                  : "Writing... ") + progress);

    upgradeOutput.clear();
}

#endif
