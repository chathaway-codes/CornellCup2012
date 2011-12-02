<?php

class IndexController extends Zend_Controller_Action
{

    public function init()
    {
        /* Initialize action controller here */
    }

    public function indexAction()
    {
        // action body
    }

    public function addactionAction()
    {
        $form = new Application_Form_AddAction;
        
        if($this->getRequest()->isPost()) {
            if($form->isValid($this->getRequest()->getPost())) {
                $actionTable = new Zend_Db_Table('DRK_Action');
                $actionTable->createRow($form->getValues())->save();
                $form->reset();
                echo "<b>Action saved!</b>";
            }
        }
        
        $this->view->form = $form;
    }

    public function addingredientAction()
    {
        $form = new Application_Form_AddIngredient();
        
        if($this->getRequest()->isPost()) {
            if($form->isValid($this->getRequest()->getPost())) {
                $ingredientTable = new Zend_Db_Table('DRK_Ingredient');
                $ingredientTable->createRow($form->getValues())->save();
                $form->reset();
                echo "<b>Ingredient Saved</b>";
            }
        }
        
        $this->view->form = $form;
    }

    public function adddrinkAction()
    {
        $form = new Application_Form_AddDrink;
        
        if($this->getRequest()->isPost()) {
            if($form->isValid($this->getRequest()->getPost())) {
                $recipeTable = new Zend_Db_Table("DRK_Recipe");
                $recipeId = $recipeTable->createRow(array(
                            "Name" => $form->getValue("Name"),
                            "Description" => $form->getValue("Description")
                        ))->save();
                // Then the tricky part....
                $stepTable = new Zend_Db_Table("DRK_Step");
                for($i=1; $i < 15; $i++) {
                    if($form->getValue("Action$i") == '0')
                        break;
                    $stepTable->createRow(array(
                        "DRK_Recipe_ID" => $recipeId,
                        "DRK_Action_ID" => $form->getValue("Action$i"),
                        "DRK_Ingredient_ID" => $form->getValue("Ingredient$i") 
                            == '0' ? null : $form->getValue("Ingredient$i"),
                        "IngredientQuantity" => $form->getValue("IngredientQuantity$i") 
                            == "" ? null : $form->getValue("IngredientQuantity$i")
                    ))->save();
                }
                $form->reset();
                echo "<b>Drink saved!</b>";
            }
        }
        
        $this->view->form = $form;
    }


}







