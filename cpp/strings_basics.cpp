//
// Created by rasheed on 2026/11/09.
//
#include <fstream>
#include <print>
#include <ranges>
#include <unordered_map>

#include "utils.h"

static auto generate_dna_sequence(const size_t& n) -> std::string
{
    std::string result = {};
    result.reserve(n);

    for (size_t i = 0; i < n; ++i)
    {
        result += get_random_from_str("ACGT");
    }

    return result;
}

static auto longest_common_prefix(const std::string_view& str1, const std::string_view& str2) -> std::string
{
    size_t i = 0;

    for (auto [s1, s2] : std::ranges::views::zip(str1, str2)
    )
    {
        if (s1 != s2) { break; }
        ++i;
    }

    return std::string{str1.substr(0, i)};
}

static auto reverse_complement(const std::string_view& sequence) -> std::string
{
    std::string rev_comp{};
    rev_comp.reserve(sequence.size());
    std::unordered_map<char, char> complements = {
        {'A', 'T'},
        {'C', 'G'},
        {'G', 'C'},
        {'T', 'A'},
        {'N', 'N'},
    };

    for (auto base : sequence)
    {
        rev_comp += complements[base];
    }

    std::reverse(rev_comp.begin(), rev_comp.end());

    return rev_comp;
}

static auto read_genome(const std::string& filename) -> std::string
{
    std::ifstream input_file(filename);
    ADS_ASSERT(input_file.is_open(), "File must be opened");
    std::string genome_sequence = {};
    std::string line = {};
    while (std::getline(input_file, line))
    {
        if (line[0] != '>' && not line.empty()) genome_sequence.append(strip_whitespace(line));
    }

    return genome_sequence;
}

static auto generate_reads_from_genome(const std::string_view& genome, const size_t num_of_reads,
                                       const size_t read_length) -> std::vector<std::string>
{
    std::vector<std::string> reads = {};

    for (size_t i = 0; i < num_of_reads; ++i)
    {
        std::mt19937 generator(std::random_device{}());
        std::uniform_int_distribution<size_t> idx(0, genome.size() - read_length);
        const auto start = idx(generator);
        reads.emplace_back(genome.substr(start, read_length));
    }
    return reads;
}

static auto read_fastq(const std::string& filename) -> std::vector<std::tuple<std::string, std::string>>
{
    std::ifstream input_file(filename);
    ADS_ASSERT(input_file.is_open(), "File must be opened");
    std::vector<std::tuple<std::string, std::string>> reads = {};
    std::string header, sequence, separator, quality = {};

    while (std::getline(input_file, header))
    {
        ADS_ASSERT(
            std::getline(input_file,sequence) &&
            std::getline(input_file,separator) &&
            std::getline(input_file,quality), "Fastq file should not be truncated"
        );

        const auto seq = strip_whitespace(sequence);
        const auto qual = strip_whitespace(quality);

        ADS_ASSERT(seq.length() == qual.length(),
                   "quality and sequence must be of the same length");

        reads.emplace_back(
            seq, qual
        );
    }

    return reads;
}

static auto naive(const std::string_view& pattern, const std::string_view& text) -> std::vector<size_t>
{
    std::vector<size_t> occurrences = {};

    for (size_t i = 0; i < text.size() - pattern.size() + 1; ++i)
    {
        bool match = true;
        for (size_t j = 0; j < pattern.size(); ++j)
        {
            if (text[i + j] != pattern[j])
            {
                match = false;
                break;
            }
        }
        if (match) { occurrences.push_back(i); }
    }

    return occurrences;
}

auto main() -> int
{
    auto dna1 = generate_dna_sequence(5);
    auto dna2 = generate_dna_sequence(5);

    std::println("DNA String 1: {}", dna1);
    std::println("DNA String 2: {}", dna2);
    ADS_ASSERT(reverse_complement(reverse_complement(dna2)) == dna2, "reverse complement should be idempotent");
    std::println("Reverse Complement of DNA String 2: {}", reverse_complement(dna2));

    std::println("Longest common prefix: {}", longest_common_prefix(dna1, dna2));

    auto genome = read_genome("../data/lambda_virus.fa");
    std::println("Genome: {}", genome);

    {
        auto reads = read_fastq("../data/SRR835775_1.first1000.fastq");

        for (auto [sequence, quality] : reads)
        {
            std::println("Sequence: {}", sequence);
            std::println("Quality: {}", quality);
        }
    }
    // naive search with artificial reads
    {
        size_t num_matched = 0;
        const auto reads = generate_reads_from_genome(genome, 100, 100);
        for (auto& read : reads)
        {
            if (auto matches = naive(read, genome); !matches.empty()) { num_matched++; }
        }

        std::println("{}/{} reads matched exactly", num_matched, reads.size());
    }

    // naive search with real reads (phix) using the raw read
    {
        const auto phix_genome = read_genome("../data/phix.fa");
        size_t num_matched = 0;
        const auto phix_reads = read_fastq("../data/ERR266411_1.first1000.fastq");

        for (auto& [read, _] : phix_reads)
        {
            // NOTE(abdulrasheed): we use the first 30 substring because the whole string matching gives 7/ 1000 matches
            const auto seq = read.substr(0, 30);
            if (auto matches = naive(seq, phix_genome); !matches.empty()) { num_matched++; }
        }

        std::println("{}/{} phix_reads matched exactly", num_matched, phix_reads.size());
    }

    // naive search with real reads (phix) using the raw read and with the reverse complement
    {
        const auto phix_genome = read_genome("../data/phix.fa");
        size_t num_matched = 0;
        const auto phix_reads = read_fastq("../data/ERR266411_1.first1000.fastq");

        for (auto& [read, _] : phix_reads)
        {
            const auto seq = read.substr(0, 30);
            auto matches = naive(seq, phix_genome);
            auto rev_comp_matches = naive(reverse_complement(seq), phix_genome);
            matches.insert(matches.end(), rev_comp_matches.begin(), rev_comp_matches.end());
            if (!matches.empty()) { num_matched++; }
        }

        std::println("{}/{} phix_reads matched exactly", num_matched, phix_reads.size());
    }

    return 0;
}
