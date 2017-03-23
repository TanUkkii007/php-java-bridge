package com.github.tanukkii007.php.core;

import org.junit.Test;
import org.scalatest.junit.JUnitSuite;
import static org.junit.Assert.*;

public class PHPTest extends JUnitSuite {

    @Test
    public void test() {
        int result = PHP.eval("print_r($argv);", "a", "b", "c");
        assertEquals(0, result);
    }

    @Test
    public void evaluateBoolean() {
        boolean resultTrue = PHP.evaluateBoolean("true", "a", "b", "c");
        assertEquals(true, resultTrue);
        boolean resultFalse = PHP.evaluateBoolean("false", "a", "b", "c");
        assertEquals(false, resultFalse);
    }


}
