<?php 


$url = 'http://api.smartcitizen.me/v0.0.1/26515ecad271367264ff71ee4c2e4fb3f24191bb/lastpost.json';
//  Initiate curl
$ch = curl_init();
// Disable SSL verification
curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
// Will return the response, if false it print the response
curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
// Set the url
curl_setopt($ch, CURLOPT_URL,$url);
// Execute
$result=curl_exec($ch);
// Closing
curl_close($ch);

// Will dump a beauty json :3
$result_parse = json_decode($result, true);
//echo "<pre>" . print_r($result_parse,true) . "</pre>";

$temp  = $result_parse['devices'][0]['posts']['temp'];
$hum   = $result_parse['devices'][0]['posts']['hum'];
$co    = $result_parse['devices'][0]['posts']['co'];
$no2   = $result_parse['devices'][0]['posts']['no2'];
$light = $result_parse['devices'][0]['posts']['light'];
$noise = $result_parse['devices'][0]['posts']['noise'];
$bat   = $result_parse['devices'][0]['posts']['bat'];
$panel = $result_parse['devices'][0]['posts']['panel'];

echo $temp." ".$hum." ".$co." ".$no2." ".$light." ".$noise." ".$bat;
?>
