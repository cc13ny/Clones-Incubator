<?php

	/** version 0.0.1
	 *  author: cchen
	 *  last modified date: 2015/03/03
	 *  php version: 5.6.6
	 */


	/**Arguments Checker**/
	
	if($argc != 4){
	  //if only one file name is given *
		//if no file names are given *
		
		echo "\n";
		echo "-- Run command line: php <php_filename> <task_option> <input_filename> <output_filename>\n";
		echo "  -- <php_filename>: name of the php file you want to run, i.e. cchen1.php\n";
		echo "  -- <task_option>: 1 or 2\n";
		echo "  -- <input_filename>: name of the input file. For example, input.txt\n";
		echo "  -- <output_filename>: name of the output file. For example, output.txt\n";
		exit; // should exist 
	}

	
	/**Get parameters**/
	$task_option = $argv[1];
	$input_filename = $argv[2];
	$output_filename = $argv[3];
	
	/**Read input file**/
	$input = file_get_contents($input_filename); 
	
	/**Tasks 1 & 2**/
	if($input != FALSE){
		
		/***Task 1: String Rot13 Coder***/
		if($task_option == 1){
			/*Task 1*/
		
			/**Rot13 Coding**/
			
			for($i=0; $i<strlen($input); $i++){
				$ch = $input[$i];
			      
      	if(ctype_lower($ch)){
      		$input[$i] = chr(ord("a") + (ord($ch) - ord("a") + 13) % 26);
      	}
      	else if (ctype_upper($ch)){
      		$input[$i] = chr(ord("A") + (ord($ch) - ord("A") + 13) % 26);
      	}
      	else if(ctype_digit($ch)){
      		$input[$i] = ($ch + 5) % 10;
      	}
    	}
		} /***Task 2: Caesar cypher***/
		else if($task_option == 2){
			/*Task 2*/
			$map = array();
			$map['a'] = 'n';
			$map['b'] = 'o';
			$map['c'] = 'p';
			$map['d'] = 'q';
			$map['e'] = 'r';
			// Or you can do so: $map = array('a' => 'n', 'b' => 'o', 'c' => 'p', 'd' => 'q', 'e' => 'r');
			
			
			for($i=0; $i<strlen($input); $i++){
				$ch = $input[$i];
				if (array_key_exists($ch, $map)) {
					$input[$i] = $map[$ch];
				}
			}
		
		}
		else{
			echo "\n";
			echo "-- Run the command line: php <php_filename> <task_option> <input_filename> <output_filename>\n";
			echo "   -- Pay attention that <task_option> should be 1 or 2\n";
		}
	}
	else{
		 //if the input file doesn't exist ***
		 echo "\n";
		 echo " The input file named '".$input_filename."' doesn't exist!\n";
		 exit;
	}
	
	/**Write output file**/
	file_put_contents($output_filename, $input);
?>