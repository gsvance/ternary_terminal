/* Interpreter for Malbolge.                                          */
/* '98 Ben Olmstead.                                                  */
/*                                                                    */
/* Malbolge is the name of Dante's Eighth circle of Hell.  This       */
/* interpreter isn't even Copylefted; I hereby place it in the public */
/* domain.  Have fun...                                               */
/*                                                                    */
/* Note: in keeping with the idea that programming in Malbolge is     */
/* meant to be hell, there is no debugger.                            */
/*                                                                    */
/* By the way, this code assumes that short is 16 bits.  I haven't    */
/* seen any case where it isn't, but it might happen.  If short is    */
/* longer than 16 bits, it will still work, though it will take up    */
/* considerably more memory.                                          */
/*                                                                    */
/* If you are compiling with a 16-bit Intel compiler, you will need   */
/* >64K data arrays; this means using the HUGE memory model on most   */
/* compilers, but MS C, as of 8.00, possibly earlier as well, allows  */
/* you to specify a custom memory-model; the best model to choose in  */
/* this case is /Ashd (near code, huge data), I think.                */

use std::env;
use std::fs;
use std::vec;
//#include <stdio.h>
//#include <stdlib.h>
//#include <ctype.h>
//#include <malloc.h>
//#include <string.h>

//#ifdef __GNUC__
//static inline
//#endif
//void exec( unsigned short *mem );

//#ifdef __GNUC__
//static inline
//#endif
//unsigned short op( unsigned short x, unsigned short y );

const xlat1: &str =
  "+b(29e*j1VMEKLyC})8&m#~W>qxdRp0wkrUo[D7,XTcA\"lI\
  .v%{gJh4G\\-=O@5`_3i<?Z';FNQuY]szf$!BS/|t:Pn6^Ha";

const xlat2: &str =
  "5z]&gqtyfr$(we4{WP)H-Zn,[%\\3dL+Q;>U!pJS72FhOA1C\
  B6v^=I_0/8|jsb9m<.TVac`uY*MK'X~xDl}REokN:#?G\"i@";

fn main() {
  let mut i = 0;
  let argv: Vec<String> = env::args().collect();
  let argc = argv.len();
  if (argc != 2) {
    eprintln!("invalid command line");
    std::process::exit(1);
  }
  let f = fs::File::open(argv[1], "r");
  if let Err(_) = f {
    eprintln!("can't open file");
    std::process::exit(1);
  }
  let f = f.unwrap();
  let mem: Box<[u16; 59049]>;
  //if ( mem == NULL )
  //{
  //  fclose( f );
  //  fputs( "can't allocate memory\n", stderr );
  //  return ( 1 );
  //}
  while ( ( x = getc( f ) ) != EOF )
  {
    if (isspace(x)) { continue }
    if ( x < 127 && x > 32 )
    {
      if ( strchr( "ji*p</vo", xlat1[( x - 33 + i ) % 94] ) == NULL )
      {
        fputs( "invalid character in source file\n", stderr );
        free( mem );
        fclose( f );
        return ( 1 );
      }
    }
    if ( i == 59049 )
    {
      fputs( "input file too long\n", stderr );
      free( mem );
      fclose( f );
      return ( 1 );
    }
    mem[i++] = x;
  }
  fclose( f );
  while ( i < 59049 ) {
    mem[i] = op( mem[i - 1], mem[i - 2] );
    i++;
  }
  exec( mem );
  free( mem );
  return ( 0 );
}

//#ifdef __GNUC__
//static inline
//#endif
fn exec(mem: Box<[u16; 59049]>) {
  let (a, c, d): (u16, u16, u16) = (0, 0, 0);
  loop {
    if (mem[c] < 33 || mem[c] > 126) { continue }
    match (xlat1[( mem[c] - 33 + c ) % 94]) {
      'j' => d = mem[d],
      'i' => c = mem[d],
      '*' => a = mem[d] = mem[d] / 3 + mem[d] % 3 * 19683,
      'p' => a = mem[d] = op( a, mem[d] ),
      '<' => {
        if '\n' != 10 {
          if ( x == 10 ) {
            putc( '\n', stdout )
          }
        }
        putc( a, stdout );
      },
      '/' =>
        x = getc( stdin );
#if '\n' != 10
        if ( x == '\n' ) a = 10; else
#endif
        if ( x == EOF ) a = 59048; else a = x;
        break;
      case 'v': return;
    }
    mem[c] = xlat2[mem[c] - 33];
    if ( c == 59048 ) c = 0; else c++;
    if ( d == 59048 ) d = 0; else d++;
  }
}

//#ifdef __GNUC__
//static inline
//#endif
fn op(x: u16, y: u16) -> u16 {
  let i: u16 = 0;
  let p9: [u16; 5] =
    [1, 9, 81, 729, 6561];
  let o: [[u16; 9]; 9] =
    [
      [ 4, 3, 3, 1, 0, 0, 1, 0, 0 ],
      [ 4, 3, 5, 1, 0, 2, 1, 0, 2 ],
      [ 5, 5, 4, 2, 2, 1, 2, 2, 1 ],
      [ 4, 3, 3, 1, 0, 0, 7, 6, 6 ],
      [ 4, 3, 5, 1, 0, 2, 7, 6, 8 ],
      [ 5, 5, 4, 2, 2, 1, 8, 8, 7 ],
      [ 7, 6, 6, 7, 6, 6, 4, 3, 3 ],
      [ 7, 6, 8, 7, 6, 8, 4, 3, 5 ],
      [ 8, 8, 7, 8, 8, 7, 5, 5, 4 ],
    ];
  for j = 0..5 {
    i += o[y / p9[j] % 9][x / p9[j] % 9] * p9[j];
  }
  return i;
}

