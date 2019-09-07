#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <memory>
#include <experimental/filesystem>

#include <args.hxx>

#include <EmbGen/EmbGen.hpp>


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

    std::string config_filename = args::get(config_arg);
    std::string template_filename = args::get(template_arg);
    std::string output_folder = args::get(output_folder_arg);
    std::string appendages_folder = args::get(appendages_arg);

    struct stat output_folder_stat;
    if (stat(output_folder.c_str(), &output_folder_stat) != 0 && !(output_folder_stat.st_mode & S_IFDIR))
    {
        std::cerr << "The output folder " << output_folder << " doesn't exist." << std::endl;
        return EXIT_FAILURE;
    }

    struct stat appendages_folder_stat;
    if (stat(appendages_folder.c_str(), &appendages_folder_stat) != 0 && !(output_folder_stat.st_mode & S_IFDIR))
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
        generator = std::make_unique<emb::gen::EmbGen>(config, appendages_folder, ino_template);
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

    std::experimental::filesystem::create_directory(output_folder + "/main");

    std::ofstream source_file(output_folder + "/main/main.ino");
    source_file << source;
    source_file.close();

    std::ofstream core_config_file(output_folder + "/core_config.json");
    core_config_file << core_config;
    core_config_file.close();
}