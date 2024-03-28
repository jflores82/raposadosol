<?PHP 

// CSV to C (SDCC for Master System) Array Converter
// Developed by tibonev to use in Raposa do Sol 
// Undocumented //

require_once('class.csvreader.php');
$csvfile = 'level.csv'; // map file
$wall_tile = "0";
$wall_num = 0;
$pell_tile = "2";
$pell_num = 0;

$csv = new csvreader($csvfile);
$file = $csv->ReadCSV(1, ",", " ");
$i = 0;
foreach($file as $y=>$value) { 
	$y = $y * 8;
	foreach($value as $x=>$val) { 
		if($val == $wall_tile) { 
			$wall_num++;
			$x = $x*8;
			echo "walls_x_y[$i][0] = $x;    ";
			echo "walls_x_y[$i][1] = $y;<br>";
			$i++;
		}
	}
}
	
echo "level_walls_num=$wall_num;<br><br><br>";

$i = 0;
foreach($file as $y=>$value) { 
	$y = $y * 8;
	foreach($value as $x=>$val) { 
		if($val == $pell_tile) { 
			$pell_num++;
			$x = $x*8;
			echo "pellets_x_y[$i][0] = $x;    ";
			echo "pellets_x_y[$i][1] = $y;<br>";
			$i++;
		}
	}
}
echo "level_pellet_num=$pell_num;";


?>