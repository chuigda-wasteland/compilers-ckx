/**
    The Opensource Compiler CKX -- for my honey ChenKX
    Copyright (C) 2017  CousinZe

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see http://www.gnu.org/licenses/.
  */

#include "file_reader.hpp"
#include "ckx_parser.hpp"
#include "ckx_ast_node.hpp"

#include <cstdio>

int main(int argc, const char* argv[])
{
    using namespace ckx;

    if (argc != 2)
    {
        std::printf("Requires input file.\n");
        return -1;
    }

    std::FILE* fp = std::fopen(argv[1], "r");
    if (fp == nullptr)
    {
        std::printf("File not found.\n");
        return -1;
    }

    we::we_fp_reader reader(fp);
    we::we_fp_writer writer(stdout);
    ckx_token_stream *stream = new ckx_token_stream(reader);

    ckx_parser parser;
    auto parse_result = parser.parse(stream);
    parse_result.trans_unit->ast_dump(writer, 0);

    for (auto &error : parse_result.error_list)
    {
        std::printf("At (%hu,%hu):", error.rng.begin_line, error.rng.begin_col);
        std::printf("Error -- %s \n", error.desc.get().c_str());
    }

    delete stream;

    std::fclose(fp);
    return 0;
}
