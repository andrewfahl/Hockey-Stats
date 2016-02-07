<?php
/**
 * @file
 * Hockey Stats Module Location Pages and Functions
 * 
 */

/**
 * Location Main Landing Page
 */
function hockey_stats_location_landing_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_location_landing');
}

function hockey_stats_location_add_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_location_add');
}

/**
 * Location menu item edit page
 */
function hockey_stats_location_edit_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_location_edit');
}

/**
 * Location menu item delete page
 */
function hockey_stats_location_delete_page() {
  header('Content-type: text/plain; charset=UTF-8');
  header('Content-Disposition: inline');
  return drupal_get_form('hockey_stats_location_delete');
}

/**
 * Location menu item landing page
 */
function hockey_stats_location_landing() {

  $header = array(
       'name' => t('Name'),
       'address' => t('Address'),
       'edit' => t('Edit'),
       'delete' => t('Delete'),
  );

  $options = array();

  $application = new Application();
  $locations = $application->getLocations();

  foreach ($locations as $location) {

      $edit_button = array(
         '#type' => 'button',
         '#value' => t('Edit'),
         '#attributes' => array('onclick' => 'locationEdit(' . $location->getLocationId() . ');'),
    );

    $delete_button = array(
         '#type' => 'button',
         '#value' => t('Delete'),
         '#attributes' => array('onclick' => 'locationDelete(' . $location->getLocationId() . ');'),
    );

    $formatted_address = Utility::getAddressFormatted($location->getAddress(), 
      $location->getAddress2(), $location->getCity(), $location->getState(), 
      $location->getZip());
      
    $options[$location->getLocationId()] = array(
         'name' => t($location->getName()),
         'address' => t($formatted_address),
         'edit' => array('data' => $edit_button),
         'delete' => array('data' => $delete_button),
    );
  }

  $form['addLink'] = array(
       '#type' => 'link',
       '#title' => t('Add new location'),
       '#href' => 'base/location/add',
  );

  $form['table'] = array(
       '#theme' => 'table',
       '#header' => $header,
       '#rows' => $options,
       '#id' => 'locationTable',
       '#empty' => t('No locations found'),
  );

  $form['formAction'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'formAction'),
  );

  $form['locationId'] = array(
       '#type' => 'hidden',
       '#value' => '',
       '#attributes' => array('id' => 'locationId'),
  );

  $form['#attached']['js'] = array(
       drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );

  return $form;
}

/**
 * Location add page content
 */
function hockey_stats_location_add($form_state = NULL) {

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Add Location'),
  );

  $form['form']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('Name'),
       '#size' => 50,
       '#maxlength' => 255,
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
  );

  $form['form']['address'] = array(
       '#type' => 'textfield',
       '#title' => t('Address'),
       '#size' => 50,
       '#maxlength' => 255,
  );
  
  $form['form']['address2'] = array(
       '#type' => 'textfield',
       '#title' => t('Address2'),
       '#size' => 50,
       '#maxlength' => 255,
  );
  
  $form['form']['city'] = array(
       '#type' => 'textfield',
       '#title' => t('City'),
       '#size' => 30,
       '#maxlength' => 255,
  );
  
  $form['form']['state'] = array(
       '#type' => 'textfield',
       '#title' => t('State'),
       '#size' => 5,
       '#maxlength' => 255,
  );
  
  $form['form']['zip'] = array(
       '#type' => 'textfield',
       '#title' => t('Zip'),
       '#size' => 10,
       '#maxlength' => 255,
  );

  $form['form']['locationId'] = array(
       '#type' => 'hidden',
       '#value' => '0',
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );
  
  return $form;
}

function hockey_stats_location_edit($form, &$form_state) {

  // if it was submitted via the form itself
  if ($form_state['input']['form_id'] == 'hockey_stats_location_edit') {
    $location = hockey_stats_build_location_from_form($form_state);
  }
  elseif (isset($form_state['input']['locationId'])) {
    $location = new Location($form_state['input']['locationId']);
    $location->build();
  }
  else {
    throw(new Exception("Location not found"));
  }

  $form['form'] = array(
       '#type' => 'fieldset',
       '#title' => t('Edit Location'),
  );

  $form['form']['name'] = array(
       '#type' => 'textfield',
       '#title' => t('Name'),
       '#size' => 50,
       '#maxlength' => 255,
       '#value' => $location->getName(),
       '#attributes' => array(
         'class' => array(
           'setFocusOnLoad' 
         ),
       ),
  );

  $form['form']['address'] = array(
       '#type' => 'textfield',
       '#title' => t('Address'),
       '#size' => 50,
       '#maxlength' => 255,
       '#value' => $location->getAddress(),
  );
  
  $form['form']['address2'] = array(
       '#type' => 'textfield',
       '#title' => t('Address2'),
       '#size' => 50,
       '#maxlength' => 255,
       '#value' => $location->getAddress2(),
  );
  
  $form['form']['city'] = array(
       '#type' => 'textfield',
       '#title' => t('City'),
       '#size' => 30,
       '#maxlength' => 255,
       '#value' => $location->getCity(),
  );
  
  $form['form']['state'] = array(
       '#type' => 'textfield',
       '#title' => t('State'),
       '#size' => 5,
       '#maxlength' => 255,
       '#value' => $location->getState(),
  );
  
  $form['form']['zip'] = array(
       '#type' => 'textfield',
       '#title' => t('Zip'),
       '#size' => 10,
       '#maxlength' => 255,
       '#value' => $location->getZip(),
  );

  $form['form']['locationId'] = array(
       '#type' => 'hidden',
       '#value' => $location->getLocationId(),
  );

  $form['form']['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Submit',
  );

  $form['form']['cancel'] = Utility::getCancelButton();

  $form['#attached']['js'] = array(
    drupal_get_path('module', 'hockey_stats') . '/includes/main.js',
  );
  
  return $form;
  
}

function hockey_stats_location_delete($form, &$form_state) {

  if (isset($form_state['input']['locationId'])) {
    $location = new Location($form_state['input']['locationId']);
    $location->build();
  }
  else {
    throw(new Exception("Location not found"));
  }

  $form['heading'] = array(
       '#prefix' => '<p>',
       '#markup' => 'Are you sure you want to delete the "' . $location->getName() . '" Location?  All data will be lost!',
       '#suffix' => '</p>',
  );

  $form['locationId'] = array(
       '#type' => 'hidden',
       '#value' => $location->getLocationId(),
  );

  $form['submit'] = array(
       '#type' => 'submit',
       '#value' => 'Delete Location',
  );

  $form['cancel'] = Utility::getCancelButton();

  return $form;
}

function hockey_stats_location_add_validate($form, &$form_state) {
  hockey_stats_validate_location($form, $form_state);
}

function hockey_stats_location_edit_validate($form, &$form_state) {
  hockey_stats_validate_location($form, $form_state);
}

function hockey_stats_validate_location($form, &$form_state) {
 if ($form_state['input']['op'] == "Submit") {
    $location = hockey_stats_build_location_from_form($form_state);
    
    $application = new Application();

    $dup_location_name = $application->getLocationByName($location->getName(), $location->getLocationId());

    if ($location->getName() == '') {
      form_set_error('name', 'Name is required');
    }
  
    elseif (isset($dup_location_name)) {
      form_set_error('name', 'The Location Name "' . $location->getName() . '" is already in use');
    }
  }
}

function hockey_stats_location_edit_submit($form, &$form_state) {
  hockey_stats_location_add_submit($form, $form_state);
}

function hockey_stats_location_add_submit($form, &$form_state) {
  if ($form_state['input']['op'] == "Submit") {
    $location = hockey_stats_build_location_from_form($form_state);
    $location->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());

    $location_original = new Location($location->getLocationId());
    $location_original->build();

    if (!isset($location_original)) {
      $location_original = new Location();
    }

    if (!$location->equals($location_original)) {
      $location->save();
    }

    drupal_set_message(t('The location has been saved'));
  }
  $form_state['redirect'] = array('base/location', );
}


function hockey_stats_location_delete_submit($form, &$form_state) {

  if ($form_state['input']['op'] == "Delete Location") {
    $location_id = $form_state['input']['locationId'];

    if (isset($location_id)) {
      $location = new Location($location_id);
      $location->build();
    }
    else {
      throw(new Exception("Location not found"));
    }

    $location->setSavedDateTimeAndUser(Utility::currentDate(), Utility::currentUserId());
    $location->delete();

    drupal_set_message(t('The location has been deleted'));
  }

  $form_state['redirect'] = array('base/location', );
}

function hockey_stats_build_location_from_form(&$form_state) {

  $location_id = $form_state['input']['locationId'];
  $name = trim($form_state['input']['name']);
  $address = trim($form_state['input']['address']);
  $address2 = trim($form_state['input']['address2']);
  $city = trim($form_state['input']['city']);
  $state = trim($form_state['input']['state']);
  $zip = trim($form_state['input']['zip']);
  
  $location = new Location($location_id, $name, $address, $address2, $city, $state, $zip);

  return $location;
}