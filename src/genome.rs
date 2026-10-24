use std::{collections::HashMap, fs::read_to_string};

pub struct Genome {
    pub sequence: String,
}

impl Genome {
    pub fn new(filename: &str) -> Self {
        // NGC: not gonna crash :)
        let sequence = read_to_string(filename)
            .unwrap()
            .lines()
            .filter(|seq| !seq.starts_with('>'))
            .map(|seq| String::from(seq.trim()))
            .collect();

        Self { sequence }
    }

    pub fn get_counts(&self) -> HashMap<char, u32> {
        let mut counts = HashMap::from([('A', 0), ('C', 0), ('G', 0), ('T', 0)]);
        for ch in self.sequence.chars() {
            counts.entry(ch).and_modify(|base| *base += 1);
        }

        counts
    }
}
