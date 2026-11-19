// Checks that the usage-reporting switch (../src/header/usageReporting.h) fails closed: once a
// player has turned reporting off, in whatever words, nothing turns it back
// on. Needs no SDL. Every reporter here is pointed at a closed loopback port,
// so nothing reaches the trace service.
//
//   g++ -std=c++17 -Wall -Wextra -Werror -pthread test/usageReportingTest.cpp -o usageReportingTest
//   ./usageReportingTest
#include "../src/header/usageReporting.h"

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using std::string;

static string readFile(const string &path) {
    std::ifstream in(path.c_str());
    std::ostringstream text;
    text << in.rdbuf();
    return text.str();
}

static void testEnabledValues(const string &directory) {
    string path = directory + "/values.conf";
    const char *offValues[] = {"false", "off", "no", "0", "FALSE", "Off", "", "disabled", "flase"};
    for (const char *value : offValues) {
        std::ofstream(path.c_str()) << "enabled=" << value << "\n";
        assert(!usage_reporting::readSettings(path).enabled);
    }
    const char *onValues[] = {"true", "on", "yes", "1", "TRUE", " On "};
    for (const char *value : onValues) {
        std::ofstream(path.c_str()) << "enabled=" << value << "\n";
        assert(usage_reporting::readSettings(path).enabled);
    }
    // no enabled line at all keeps the default
    std::ofstream(path.c_str()) << "# nothing set\n";
    assert(usage_reporting::readSettings(path).enabled);
    // no file at all is the first-run state
    std::remove(path.c_str());
    assert(!usage_reporting::readSettings(path).exists);
    assert(usage_reporting::readSettings(path).enabled);
}

static void testUnreadableSettingsTurnReportingOff(const string &directory) {
    // a directory where the settings file should be: there, but not readable as one
    string path = directory + "/unreadable.conf";
    assert(mkdir(path.c_str(), 0755) == 0);
    usage_reporting::Settings settings = usage_reporting::readSettings(path);
    assert(settings.exists);
    assert(!settings.enabled);
    std::remove(path.c_str());
}

static void testReporterKeepsAnOptOut(const string &home) {
    setenv("HOME", home.c_str(), 1);
    setenv("XDG_CONFIG_HOME", (home + "/.config").c_str(), 1);
    setenv("XDG_DATA_HOME", (home + "/.local/share").c_str(), 1);
    setenv(usage_reporting::ENV_ENDPOINT, "http://127.0.0.1:9", 1);
    unsetenv("TRACE_USAGE_REPORTING");
    unsetenv("DO_NOT_TRACK");
    unsetenv("TRACE_INSTALL_ID");
    string settings = usage_reporting::settingsPath();
    assert(usage_reporting::makeDirectories(usage_reporting::configDirectory()));

    const char *offValues[] = {"off", "no", "0", "", "flase"};
    for (const char *value : offValues) {
        string contents = string("enabled=") + value + "\n";
        std::ofstream(settings.c_str()) << contents;
        std::ostringstream notices;
        {
            usage_reporting::UsageReporter reporter(notices);
            assert(!reporter.isEnabled());
            reporter.reportStartup();
        }
        assert(notices.str().empty());
        assert(readFile(settings) == contents); // never rewritten
        assert(!std::ifstream(usage_reporting::installIdPath().c_str()).good());
    }

    // an unreadable settings path is off, prints nothing and is left alone
    std::remove(settings.c_str());
    assert(mkdir(settings.c_str(), 0755) == 0);
    {
        std::ostringstream notices;
        usage_reporting::UsageReporter reporter(notices);
        assert(!reporter.isEnabled());
        assert(notices.str().empty());
    }
    std::remove(settings.c_str());

    // and a clear "on" is on, so the checks above are not passing vacuously
    std::ofstream(settings.c_str()) << "enabled=true\n";
    {
        std::ostringstream notices;
        usage_reporting::UsageReporter reporter(notices);
        assert(reporter.isEnabled());
    }
    std::remove(usage_reporting::installIdPath().c_str());
    std::remove(usage_reporting::dataDirectory().c_str());
    std::remove(settings.c_str());
    std::remove(usage_reporting::configDirectory().c_str());
    unsetenv(usage_reporting::ENV_ENDPOINT);
}

int main() {
    char pattern[] = "/tmp/usage-reporting-test-XXXXXX";
    string directory = mkdtemp(pattern);
    testEnabledValues(directory);
    testUnreadableSettingsTurnReportingOff(directory);
    testReporterKeepsAnOptOut(directory);
    std::cout << "All usage-reporting checks passed." << std::endl;
    return 0;
}
