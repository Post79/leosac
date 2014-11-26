/**
* \file main.cpp
* \author Thibault Schueller <ryp.sqrt@gmail.com>
* \brief standard main
*/

#include <iostream>

// NOTE TCLAP is easily replacable by boost::program_options
#include <tclap/CmdLine.h>
#include <core/kernel.hpp>
#include <exception/configexception.hpp>
#include <exception/ExceptionsTools.hpp>
#include <exception/coreexception.hpp>

#include "tools/log.hpp"
#include "tools/leosac.hpp"
#include "tools/runtimeoptions.hpp"
#include "tools/unixfs.hpp"
#include "tools/unixshellscript.hpp"
#include "exception/leosacexception.hpp"

void print_usage();

using namespace Leosac::Tools;

int main(int argc, char **argv)
{
    RuntimeOptions options;
    int relaunch = 1;

    try
    {
        TCLAP::CmdLine cmd("Open Source Access Controller", ' ', Leosac::getVersionString());
        TCLAP::SwitchArg verboseSwitch("v", "verbose", "Increase verbosity", false);
        TCLAP::ValueArg<std::string> kernelFile("k", "kernel-cfg", "Kernel Configuration file", false, "", "string");

        cmd.add(verboseSwitch);
        cmd.add(kernelFile);
        cmd.parse(argc, argv);
        options.setFlag(RuntimeOptions::Verbose, verboseSwitch.getValue());
        options.setParam("kernel-cfg", kernelFile.getValue());
    }
    catch (const TCLAP::ArgException &e)
    {
        throw (LEOSACException(e.error()));
    }
    while (relaunch)
    {
        UnixShellScript backup("cp -f");

        try
        {
            Kernel kernel(Kernel::make_config(options));
            relaunch = kernel.run();
        }
        catch (const CoreException &e)
        {
            Leosac::print_exception(e);
            print_usage();
            return 1;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception propagated to main(). Will now exit." << std::endl;
            Leosac::print_exception(e);
            return 1;
        }
        catch (...)
        {
            std::cerr << "Unkown exception in main" << std::endl;
            return 1;
        }
    }
    return (0);
}

void print_usage()
{
    std::cout << "./Leosac -k path/to/config [-v]" << std::endl;
}
