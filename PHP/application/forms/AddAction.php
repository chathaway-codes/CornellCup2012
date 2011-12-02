<?php

class Application_Form_AddAction extends Zend_Form
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
        $this->addElement('submit', 'Submit', array(
            'ignore' => true
        ));
    }


}

