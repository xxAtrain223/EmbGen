#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <memory>

#include <args.hxx>

#include <EmbGen/EmbGen.hpp>

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

int main(int argc, char* argv[])
{
    args::ArgumentParser parser("EmbGen generates code for an embedded device to use with EmbMessenger.");
    args::HelpFlag help(parser, "help", "Display this help menu", { 'h', "help" }, args::Options::Single);
    args::CompletionFlag completion(parser, { "complete" });
    parser.Prog("EmbGen");

    args::ValueFlag<std::string> config_arg(parser, "CONFIG", "Location of the config json file", { 'c', "config" }, args::Options::Required | args::Options::Single);
    args::ValueFlag<std::string> template_arg(parser, "TEMPLATE", "Location of the template ino file", { 't', "template" }, args::Options::Required | args::Options::Single);
    args::ValueFlag<std::string> output_folder_arg(parser, "OUTPUT_FOLDER", "Parent folder of the folder to put all the output files", { 'o', "output_folder" }, args::Options::Required | args::Options::Single);
    args::ValueFlag<std::string> appendages_arg(parser, "APPENDAGES_FOLDER", "Folder of where to look for appendage files", { 'a', "appendages" }, args::Options::Required | args::Options::Single);

    try
    {
        parser.ParseCLI(argc, argv);
    }
    catch (args::Completion e)
    {
        std::cout << e.what();
        return 0;
    }
    catch (args::Help)
    {
        std::cout << parser;
        return 0;
    }
    catch (args::ParseError e)
    {
        std::cerr << "ParseError: " << e.what() << std::endl;
        std::cerr << parser;
        return EXIT_FAILURE;
    }
    catch (args::RequiredError e)
    {
        std::cerr << "RequiredError: " << e.what() << std::endl;
        std::cerr << parser << std::endl;
        return EXIT_FAILURE;
    }
    catch (args::ValidationError e)
    {
        std::cerr << "ValidationError: " << e.what() << std::endl;
        std::cerr << parser << std::endl;
        return EXIT_FAILURE;
    }

    fs::path config_filename = args::get(config_arg);
    fs::path template_filename = args::get(template_arg);
    fs::path output_folder = args::get(output_folder_arg);
    fs::path appendages_folder = args::get(appendages_arg);

    if (!fs::is_directory(output_folder))
    {
        std::cerr << "The output folder " << output_folder << " doesn't exist." << std::endl;
        return EXIT_FAILURE;
    }

    if (!fs::is_directory(appendages_folder))
    {
        std::cerr << "The appendages folder " << appendages_folder << " doesn't exist." << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream config_stream(config_filename);
    std::string config((std::istreambuf_iterator<char>(config_stream)),
                        std::istreambuf_iterator<char>());

    std::ifstream template_stream(template_filename);
    std::string ino_template((std::istreambuf_iterator<char>(template_stream)),
                              std::istreambuf_iterator<char>());
    
    std::unique_ptr<emb::gen::EmbGen> generator;
    try
    {
        generator = std::make_unique<emb::gen::EmbGen>(config, appendages_folder.string(), ino_template);
    }
    catch (const std::exception& e)
    {
        std::cerr << "EmbGen: Exception while initializing the generator." << std::endl << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::string source;
    try
    {
        source = generator->generateSource();
    }
    catch (const std::exception& e)
    {
        std::cerr << "EmbGen: Exception while generating the source code." << std::endl << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::string core_config;
    try
    {
        core_config = generator->generateCore();
    }
    catch (const std::exception& e)
    {
        std::cerr << "EmbGen: Exception while generating the core config." << std::endl << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    fs::create_directory(output_folder / "src");

    std::ofstream source_file(output_folder / "src/main.ino");
    source_file << source;
    source_file.close();

    std::ofstream core_config_file(output_folder / "core_config.json");
    core_config_file << core_config;
    core_config_file.close();

    fs::create_directory(output_folder / "appendages");

    for (std::string appendage : generator->getAppendageNames())
    {
        fs::copy_file(
            appendages_folder / (appendage + ".xml"),
            output_folder / "appendages" / (appendage + ".xml"),
            fs::copy_options::overwrite_existing
        );
    }
}