<?php

/*
 * 
 * Static Methods used in the hockey stats module
 * 
 */

class Utility {

  const COOKIE_LEAGUEID = 'leagueid';
  const COOKIE_SEASONID = 'seasonid';
  const COOKIE_GAMEID = 'gameid';
  
  public static function currentDate() {
      return date("Y-m-d H:i:s");
  }

  public static function formatDate($date) {
      return date("m/d/Y", strtotime($date));
  }

  public static function formatDateTime($datetime) {
      return date("m/d/Y g:i A", strtotime($datetime));
  }

  public static function format24Time($datetime) {
    return date("H:i", strtotime($datetime));
  }

  public static function currentUserId() {
      global $user;
      return $user->uid;
  }

  public static function isValidDate($date) {
      if (preg_match("/^(\d{2})\/(\d{2})\/(\d{4})$/", $date, $matches)) {
          if (checkdate($matches[1], $matches[2], $matches[3])) {
              return TRUE;
          }
      }
      return FALSE;
  }

  public static function DBFormatDate($date) {
    return date('Y-m-d H:i:s', strtotime(str_replace('-', '/', $date)));
  }

  public static function getCancelButton() {
      return array(
          '#type' => 'submit',
          '#value' => 'Cancel',
      );
  }

  public static function getAddressFormatted($address, $address2, $city, $state, $zip){

    $address .= '<br />';

    if(!empty($address2)) {
      $address .= $address2 . '<br />';
    }

    $address .= $city . ' ';
    $address .= $state . ' ' . $zip;

    return $address;
  }

  public static function getCookieValue($value) {

    $returnValue = filter_input(INPUT_COOKIE, $value);

    if(!isset($returnValue)) {
      $returnValue = NULL;
    }

    return $returnValue;
  }
  
  /*
   * sets the cookie based on the params cookie name, cookie value and the 
   * cookie duration is set to 30 days
   */
  public static function setCookieValue($cookie_name, $cookie_value) {
    setcookie($cookie_name, $cookie_value, time()+60*60*24*30); 
  }

  public static function isValidTime($time) {
    if(strtotime($time)) {
      return TRUE;
    } 

    return FALSE;
  }

  public static function isPostiveNumberZeroOrEmpty($number) {
    $returnValue = TRUE;

    if(!empty($number) && (!is_numeric($number) || $number < 0)) {
      $returnValue = FALSE;
    }

    return $returnValue;
  }

  /*
   * removes items from array that are in the $removeItems array
   */
  public static function removeValues(&$array, $removeItems) {
    foreach($removeItems as $target_value) {
      foreach ($array as $key => $value){
        if ($value == $target_value) {
            unset($array[$key]);
        }
      }
    }
  }
   
}
