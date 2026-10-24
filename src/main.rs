use ads::{
    genome::Genome,
    string_basics::{generate_dna_sequence, longest_common_prefix, reverse_complement},
};

fn main() {
    let seq1 = generate_dna_sequence(9);
    let seq2 = generate_dna_sequence(9);
    println!("Seq1: {seq1}");
    println!("Seq2: {seq2}");

    println!(
        "Longest common prefix: {}",
        longest_common_prefix(&seq1, &seq2)
    );

    println!("Reverse Complement: {}", reverse_complement(&seq1));
    println!(
        "Is reverse complement working: {}",
        reverse_complement(&reverse_complement(&seq1)) == seq1.clone()
    );

    let fasta_file = "lambda_virus.fa";

    let genome = Genome::new(fasta_file);

    println!("Genome Sequence: {}", genome.sequence);
    println!("Length: {}", genome.sequence);
    println!("Counts: {:?}", genome.get_counts());
}
