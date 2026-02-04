use std::env;
use std::process;
use std::fs;
//use std::vec;

const TRIT_VALUES: usize = 3;
const WORD_SIZE: usize = 10;
const MEMORY_SIZE: usize = TRIT_VALUES.pow(WORD_SIZE as u32);

const XLAT1: &'static str =
  "+b(29e*j1VMEKLyC})8&m#~W>qxdRp0wkrUo[D7,XTcA\"lI\
  .v%{gJh4G\\-=O@5`_3i<?Z';FNQuY]szf$!BS/|t:Pn6^Ha";

const XLAT2: &'static str =
  "5z]&gqtyfr$(we4{WP)H-Zn,[%\\3dL+Q;>U!pJS72FhOA1C\
  B6v^=I_0/8|jsb9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@";

#[derive(Clone, Copy)]
enum Trit {
    Zero,
    One,
    Two,
}

#[derive(Clone, Copy)]
struct Word {
    trits: [Trit; WORD_SIZE],
}

impl Word {
    fn new(value: u16) -> Word {
        let mut word = Word {
            trits: [Trit::Zero; WORD_SIZE],
        };
        let mut value = value;
        for i in 0..WORD_SIZE {
            word.trits[i] = match value % 3 {
                0 => Trit::Zero,
                1 => Trit::One,
                2 => Trit::Two,
                _ => unreachable!(),
            };
            value /= 3;
        }
        word
    }
    
    fn as_usize(&self) -> usize {
        let mut value = 0;
        let mut b = 1;
        for i in 0..WORD_SIZE {
            value += b * match self.trits[i] {
                Trit::Zero => 0,
                Trit::One => 1,
                Trit::Two => 2,
            };
            b *= 3;
        }
        value
    }
}

fn construct_memory() -> Vec<Word> {
    let mut memory = Vec::with_capacity(MEMORY_SIZE);
    for _i in 0..MEMORY_SIZE {
        memory.push(Word::new(0))
    }
    memory
}

fn load_program(file_name: &str, mut memory: Vec<Word>) -> Result<Vec<Word>, &'static str> {
    let program = match fs::read_to_string(file_name) {
        Err(_) => return Err("can't open file"),
        Ok(s) => s,
    };

    let mut i = 0;

    for x in program.bytes() {
        if is_space(x) { continue; }
        if x < 127 && x > 32 {
            let index: usize = ((x as usize) - 33 + i) % 94;
            if "ji*p</vo".chars().any(|c| c == XLAT1.chars().nth(index).unwrap()) {
                return Err("invalid character in source file");
            }
        }
        if i == MEMORY_SIZE {
            return Err("input file too long");
        }
        memory[i] = Word::new(x as u16);
        i += 1;
    }
    while i < MEMORY_SIZE {
        memory[i] = crazy(memory[i-1], memory[i-2]);
        i += 1;
    }
    Ok(memory)
}

fn crazy(x: Word, y: Word) -> Word {
    let mut z = Word::new(0);
    for i in 0..WORD_SIZE {
        z.trits[i] = match (x.trits[i], y.trits[i]) {
            (Trit::Zero, Trit::Zero) => Trit::One,
            (Trit::Zero, Trit::One) => Trit::Zero,
            (Trit::Zero, Trit::Two) => Trit::Zero,
            (Trit::One, Trit::Zero) => Trit::One,
            (Trit::One, Trit::One) => Trit::Zero,
            (Trit::One, Trit::Two) => Trit::Two,
            (Trit::Two, Trit::Zero) => Trit::Two,
            (Trit::Two, Trit::One) => Trit::Two,
            (Trit::Two, Trit::Two) => Trit::One,
        };
    }
    z
}

fn execute_program(memory: Vec<Word>) {
    let mut a = Word::new(0);
    let mut c = Word::new(0);
    let mut d = Word::new(0);
    
    loop {
        if memory[c.as_usize()].as_usize() < 33 || memory[c.as_usize()].as_usize() > 126 { continue; }
        match XLAT1.chars().nth((memory[c.as_usize()].as_usize() - 33 + c.as_usize()) % 94).unwrap() {
            'j' => d = memory[d.as_usize()],
            'i' => c = memory[d.as_usize()],
            '*' => {
                memory[d.as_usize()] = Word::new((memory[d.as_usize()].as_usize() / 3 + memory[d.as_usize()].as_usize() % 3 * 19683) as u16);
                a = memory[d.as_usize()];
            },
            'p' => {
                memory[d.as_usize()] = crazy(a, memory[d.as_usize()]);
                a = memory[d.as_usize()];
            },
            '<' => print!("{}", a.as_usize() as char),
            '/' => {
                x = getc(stdin);
                if (x == EOF) { a = Word::new(59048); } else { a = x; }
            }
            'v' => return,
        }
    }
}

fn is_space(byte: u8) -> bool {
    match byte {
        0x20 => true,
        0x09 => true,
        0x0a => true,
        0x0b => true,
        0x0c => true,
        0x0d => true,
        _ => false,
    }
}

fn main() {
    let args: Vec<String> = env::args().collect();
    if args.len() != 2 {
        eprintln!("invalid command line\n");
        process::exit(1);
    }

    let mut memory = construct_memory();
    memory = match load_program(&args[1], memory) {
        Err(message) => {
            eprintln!("{}\n", message);
            process::exit(1);
        },
        Ok(m) => m,
    };

    execute_program(memory);
}

