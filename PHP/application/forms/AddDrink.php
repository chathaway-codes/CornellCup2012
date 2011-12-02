<?php

class Application_Form_AddDrink extends Zend_Form
{

    public function init()
    {
        $this->setMethod('POST');
        $this->addElement('text', 'Name', array(
            'label' => 'Name',
            'validators' => array(
                array('StringLength', false, array(1, 45))
            )
        ));
        
        $this->addElement('textarea', 'Description', array(
            'label' => 'Description',
            'validators' => array(
                array('StringLength', false, array(1, 4096))
            )
        ));
        
        // Get a list of the actions and recipes...
        $t = new Zend_Db_Table('DRK_Action');
        $multiAction = array('');
        foreach($t->fetchAll() as $row) {
            $multiAction[$row["ID"]] = $row["Name"];
        }
        
        $t = new Zend_Db_Table("DRK_Ingredient");
        $multiIngredient = array('');
        foreach($t->fetchAll() as $row) {
            $multiIngredient[$row["ID"]] = $row["Name"];
        }
        
        // 1
        for($i=1; $i <= 15; $i++) {
            $this->addElement('select', "Action$i", array(
                'label' => "Action $i",
                'multiOptions' => $multiAction
            ));

            $this->addElement('select', "Ingredient$i", array(
                'label' => "Ingredient $i",
                'multiOptions' => $multiIngredient
            ));
            
            $this->addElement('text', "IngredientQuantity$i", array(
                'label' => "Quantity $i",
                'validators' => array(
                    'float'
                )
            ));
        }
        
        // And place a few instance of the action/ingredient combinations
        
        $this->addElement('submit', 'Submit');
    }


}

