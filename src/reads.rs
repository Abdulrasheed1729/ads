use std::{fmt::Display, fs::read_to_string};

#[derive(Debug)]
pub struct FastqRecord {
    pub sequence: String,
    pub quality: String,
}

impl Display for FastqRecord {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        f.write_fmt(format_args!(
            "sequence: {}\nquality: {}",
            self.sequence, self.quality
        ))
    }
}

#[derive(Debug)]
pub struct SequenceReads {
    pub reads: Vec<FastqRecord>,
}

impl SequenceReads {
    pub fn new(filename: &str) -> Self {
        let mut reads: Vec<FastqRecord> = vec![];
        let s = read_to_string(filename).unwrap();
        let mut lines = s.lines();

        loop {
            // skip identifier
            lines.next();
            let seq = lines.next();
            if seq == None {
                break;
            }
            // skip '+'
            lines.next();

            let quality = lines.next();
            reads.push(FastqRecord {
                sequence: seq.unwrap().trim().to_string(),
                quality: quality.unwrap().trim().to_string(),
            });
        }
        Self { reads }
    }
}
