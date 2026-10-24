use rand::seq::IndexedRandom;
use std::collections::HashMap;
use std::iter::zip;

pub fn reverse_complement(seq: &str) -> String {
    let complements = HashMap::from([('A', 'T'), ('C', 'G'), ('G', 'C'), ('T', 'A')]);
    let mut rev_comp = String::from("");

    for base in seq.chars() {
        let ch = complements.get(&base).unwrap();
        rev_comp.push(*ch);
    }

    rev_comp
        .chars()
        .into_iter()
        .rev()
        .map(|c| c.to_string())
        .collect::<Vec<String>>()
        .join("")
}

pub fn longest_common_prefix(seq1: &str, seq2: &str) -> String {
    let mut i = 0;

    for (s1, s2) in zip(seq1.chars(), seq2.chars()) {
        if s1 == s2 {
            i = i + 1;
        } else {
            break;
        }
    }

    String::from(&seq1[0..i])
}

pub fn generate_dna_sequence(n: u32) -> String {
    let mut seq = String::from("");
    let alphabet = vec!['A', 'C', 'G', 'T'];
    for _ in 0..n {
        let choice = alphabet.choose(&mut rand::rng()).unwrap();
        seq.push(*choice);
    }
    seq
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_longest_common_prefix() {
        let seq1 = String::from("ACTGGTA");
        let seq2 = String::from("ACTAGATTGAG");

        assert_eq!(longest_common_prefix(&seq1, &seq2), String::from("ACT"));
    }

    #[test]
    fn test_reverse_complement() {
        let seq1 = generate_dna_sequence(9);
        assert_eq!(reverse_complement(&reverse_complement(&seq1)), seq1);
    }
}
