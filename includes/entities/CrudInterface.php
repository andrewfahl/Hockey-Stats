<?php

/* 
 * 
 * Interface that shows what all hockey stats objects should be able to do
 *
 */
interface CrudInterface {
    
    /*
     * Builds the object with values from the database based on its primary key
     * that has already been assigned to the object
     */
    public function build();
    
    /*
     * Saves the object in the database 
     */
    public function save();
    
    /*
     * Deletes the object in the database
     */
    public function delete();
    
    /*
     * Checks if all of the properties are the same.  Does not
     * check if they are the same instance
     */
    public function equals(CrudInterface $comparison_obj);
    
    /*
     * Sets the objects saved date time and the userId
     */
    public function setSavedDateTimeAndUser($savedDateTime, $savedUserID);
}