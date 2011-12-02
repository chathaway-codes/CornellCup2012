<?php

class Application_Form_AddIngredient extends Zend_Form
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
        
        $this->addElement('text', 'AlcoholContent', array(
            'label' => 'Alcohol Content',
            'validators' => array(
                'float'
            )
        ));
        $this->addElement('submit', 'Submit', array(
            'ignore' => true
        ));
    }


}

