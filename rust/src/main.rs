use std::env;
use regex::Regex;
use std::cmp::Ordering;

fn regex_checks(eq: &str) -> bool {
	// vive le regex
	let term = r"(\d+.)?\d+\*X\^\d";
	let full_pattern = format!(r"^[-+]?{term}([-+]{term})*=[-+]?{term}$");

	let reg = Regex::new(&full_pattern).unwrap();
	if !reg.is_match(&eq) {
		return false;
	}

	let mut degres = Vec::new();

	let reg = Regex::new(&term).unwrap();
	for re_match in reg.find_iter(&eq) {
		let s = re_match.as_str();
		let exp_index = s.find('^').unwrap();
		let deg: i32 = s[exp_index+1..].parse().unwrap();
		degres.push(deg);
	}
	for (i, deg) in degres.iter().enumerate() {
		if (i+1 != degres.len() && i != *deg as usize) || (i+1 == degres.len() && *deg != 0) {
			return false;
		}
	}
	true
}

fn get_coeffs(eq: &mut String) -> Vec<f32> {
	// separate X with '|'
	*eq = eq
		.replace("+", "|+")
		.replace("-", "|-")
		.replace("=", "=|");

	// remove '='
	eq.retain(|c| c != '=');

	// separate each term
	let mut terms: Vec<&str> = eq.split('|').collect();

	// remove empty string
	terms.retain(|t| *t != "");

	// retreive coeff from each term
	let mut coeffs = Vec::new();
	for term in terms {

		// get '*' index
		let mult_index = term.find('*').unwrap();

		// separate coeff from the term and parse it
		let coeff: f32 = term[..mult_index].parse().unwrap();

		coeffs.push(coeff);
	}

	// move right side coeff to the left
	let rhs_coeff = coeffs.pop().unwrap();
	coeffs[0] -= rhs_coeff;

	coeffs
}

fn print_polynomial_info(coeffs: &Vec<f32>) {
	let mut reduced_form = String::new();
	for (i, coeff) in coeffs.iter().enumerate() {
		reduced_form.push_str(&format!("{:+} * X^{} ", coeff, i));
	}
	reduced_form.push_str("= 0");
	if reduced_form.as_bytes()[0] == '+' as u8 {
		reduced_form.remove(0);
	}
	reduced_form = reduced_form.replace("+", "+ ").replace("-", "- ");
	if reduced_form.as_bytes()[0] == '-' as u8 {
		reduced_form.remove(1);
	}
	println!("Reduced form: {}", reduced_form);
	println!("Polynomial degree: {}", coeffs.len() -1);
}

fn solve_polynomial(coeffs: &Vec<f32>) {
	match coeffs.len() -1 {
		0 => match coeffs[0] as i32 {
			0 => println!("There an infinit number of solution."),
			_ => println!("There is no solution.")
		},
		1 => println!("The solution is: {}", -coeffs[0] / coeffs[1]),
		2 => {
			let a = coeffs[2];
			let b = coeffs[1];
			let c = coeffs[0];
			let discriminant = b.powf(2.0) - 4.0 * a * c;
			match discriminant.partial_cmp(&0.0) {
				Some(opt) => match opt {
					Ordering::Greater => println!("Discriminant is strictly positive, the two solutions are:\n{}\n{}",
						(-b + discriminant.sqrt()) / (2.0 * a),
						(-b - discriminant.sqrt()) / (2.0 * a)
					),
					Ordering::Equal => println!("Discriminant is equal to zero, the solution is:\n{}", -b / (2.0 * a) ),
					Ordering::Less => println!("Discriminant is strictly negative, there is no solution in R.")
				},
				None => println!("wtf compare two float return None")
			}
		},
		_ => println!("Sorry I cant solve this polynomial.")
	}
}

fn main() {
    let args: Vec<String> = env::args().collect();
	
	// get arg
	let mut eq = match args.get(1) {
		Some(arg) => String::from(arg),
		None => {
			println!("Missing an argument !");
			return;
		}
	};

	// remove spaces
	eq.retain(|c| c != ' ');

	if !regex_checks(&eq) {
		println!("Error: wrong format.");
		return;
	}

	let coeffs = get_coeffs(&mut eq);

	print_polynomial_info(&coeffs);
	
	solve_polynomial(&coeffs);
}