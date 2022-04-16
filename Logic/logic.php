<?php
    //TODO: non-UB for "impossible" input combinations (multiple slots selected, etc)

    $oldinput = $input;
    for ($i=0;$i<count($input);$i++) {
        $input[$i] = $oldinput[(count($input)-1)-$i]; //pudding
    }
    $card_present = array();
    $slot_selected = array();
    $card_present[2] = !$input[0];
    $slot_selected[1] = !$input[1];
    $card_present[1] = !$input[2];
    $slot_selected[0] = !$input[3];
    $card_present[0] = !$input[4];
    $slot_selected[3] = !$input[5];
    //
    $card_present[3] = !$input[7];
    //
    //
    $bank = $input[10];
    $slot_selected[2] = !$input[11];

    //figure out if we should be "here" for any slot
    $slot_here = array();
    for ($i=0;$i<4;$i++) {
        if ($slot_selected[$i] && !$card_present[$i]) {
            $slot_here[$i] = true;
        } else {
            $slot_here[$i] = false;
        }
    }

    //set a15 and a16 outputs based on selected slot (output bits 4 and 6)
    $slotaddr = 0;
    for ($i=0;$i<4;$i++) {
        if ($slot_selected[$i]) $slotaddr = $i;
    }
    $output[4] = (($slotaddr&(1<<0))>0)?true:false;
    $output[6] = (($slotaddr&(1<<1))>0)?true:false;

    //if we are "here" for the slot, set flash !cs based on the bank
    if ($slot_here[$slotaddr]) {
        if (!$bank) {
            $output[5] = true; //!csb
            $output[7] = false; //!csa
        } else {
            $output[5] = false; //!csb
            $output[7] = true; //!csa
        }
    } else {
        $output[5] = true; //!csb
        $output[7] = true; //!csa
    }

    //dummy
    $output[0] = true;
    $output[1] = true;
    $output[2] = true;
    $output[3] = true;
