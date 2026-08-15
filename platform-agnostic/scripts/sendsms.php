<?php

/* 
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

$modem_ip = '192.168.1.131';
$message = 'hellohello';
$mobile = '+91xxxxxxxxx';

$upd_header = '001100'; // SMSC information is zeo, SMS-Submit, TP-Message is itself.

// check local or international
if(substr($mobile,0,1) == '+'){
    $mobile_type = '91'; // international
    $mobile_number = substr($mobile,1); // remove plus
}else{
    $mobile_type = '81'; // local
    $mobile_number = $mobile;
}
$mobile_length = strlen($mobile_number);
if($mobile_length%2 != 0){
    $mobile_number = $mobile_number + 'F';
}
$numbers = str_split($mobile_number);
// swap;
for($i = 0 ; $i < count($numbers)/2 ; $i++){
    $temp = $numbers[2*$i];
    $numbers[2*$i] = $numbers[2*$i+1];
    $numbers[2*$i+1] = $temp;
}
$encoded_mobile = implode("",$numbers);
$mobile_len_oct = sprintf('%02X',$mobile_length);

$message_len = sprintf('%02X',strlen($message));
$_unpack = unpack('C*',$message);
$unpack = array();
$shifted = array();

$packed = array();

$shiftOffset = 0;

foreach($_unpack as $b){
    if($shiftOffset == 7){
        $shiftOffset = 0;
    }else{
        $unpack[] = $b;
        $shifted[] = ($b >> $shiftOffset);
        $shiftOffset++;        
    }
}
$encode_mask = [ 1, 3, 7, 15, 31, 63, 127 ];

$moveOffset = 1;
$moveIndex = 1;
$packIndex = 0;
for(;;){
    if($moveOffset == 8){
        $moveOffset = 1;
    }else{
        if($moveIndex != count($shifted)){

            $extractBis = $unpack[$moveIndex] & $encode_mask[$moveOffset - 1];

            $extractBis <<=  (8 - $moveOffset);

            $packed[] = $shifted[$packIndex] | $extractBis;
            $moveOffset++;
            $packIndex++;
        }else{
            $packed[] = $shifted[$packIndex];
            break;
        }
        $moveIndex++;                    
    }
}


$msg_encoded = '';
foreach($packed as $b){
    $msg_encoded = $msg_encoded.sprintf('%02X',$b);
}
$upd = $upd_header.$mobile_len_oct.$mobile_type.$encoded_mobile.'0000AA'.$message_len.$msg_encoded;
$upd_len = (strlen($upd) - 2)/2;
echo $upd_len;


echo $upd;

$socket=fsockopen('192.168.1.131',10001);
fwrite($socket, "at\r");
sleep(1);
fwrite($socket, "at\r");
sleep(1);
fwrite($socket, "at+cmgf=0\r");
sleep(1);
fwrite($socket, "at+cmgs=".$upd_len."\r");
sleep(1);
fwrite($socket, $upd.chr(26)."\r");
sleep(1);
fclose($socket);
