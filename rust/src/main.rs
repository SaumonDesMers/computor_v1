use std::env;

fn main() {
    let args: Vec<String> = env::args().collect();
	
	// let eq = match args.get(1) {
	// 	Some(eq) => eq,
	// 	None => {
	// 		println!("Missing an argument !");
	// 		return;
	// 	}
	// };

	if args.len() < 2 {
		println!("Missing an argument !");
		return;
	}

	let mut eq: String = String::from(&args[1]);
	eq.retain(|c: char| c != ' ');
	eq = dbg!(eq);

	let degree = eq.matches("X").count();
	dbg!(degree);
}