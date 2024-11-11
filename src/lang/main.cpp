#include <fstream>
#include <iostream>
#include <LX/ArgParser.hpp>
#include <LX/AST.hpp>
#include <LX/Builder.hpp>
#include <LX/Context.hpp>
#include <LX/Linker.hpp>
#include <LX/Parser.hpp>

int main(const int argc, const char* const* argv)
{
    LX::ArgParser args({
        {"help", "display help text", {"--help", "-h"}},
        {"output", "specify output file", {"--output", "-o"}, false},
        {"version", "display version info", {"--version", "-v"}},
    });
    args.Parse(argc, argv);

    if (argc == 1)
        args.Flags["help"] = true;

    if (args.Flags["help"] || args.Flags["version"])
        std::cout << "LX [version 1.0.0]" << std::endl;

    if (args.Flags["help"])
    {
        args.Print();
        return 0;
    }

    if (args.Args.empty())
    {
        std::cerr << "no input files provided" << std::endl;
        return 1;
    }

    const auto& input_filenames = args.Args;
    const auto& output_filename = args.Options["output"];

    LX::Linker linker("lx");
    for (const auto& filename : input_filenames)
    {
        std::ifstream stream(filename);
        if (!stream.is_open())
        {
            std::cerr << "failed to open file " << filename << std::endl;
            continue;
        }

        LX::Context ctx;
        LX::Builder builder(ctx, linker.IRContext(), filename);
        LX::Parser::Parse(ctx, stream, filename, [&](LX::StmtPtr&& ptr)
        {
            // ptr->Print(std::cerr) << std::endl;

            ptr->GenIR(builder);
        });
        stream.close();

        builder.Finalize();
        linker.Link(builder);
    }
    linker.Output(output_filename);
}
