<?php

    function gen($number, $max_decimals = 4)
    {
        $n = (double) $number;

        for ($decimals = 0; $decimals <= $max_decimals; ++$decimals)
        {
            // char

            $s = number_format($n, $decimals, '.', '');
            echo 'snn_require(fmt::floating_point(', $number, ', ', $decimals, ') == "', $s, '");';
            echo "\n";

            $s = number_format($n, $decimals, ',', ' ');
            echo 'snn_require(fmt::floating_point(';
            echo $number, ', ', $decimals, ", ',', ' ')";
            echo ' == "', $s, '");';
            echo "\n";

            $s = number_format(-1.0 * $n, $decimals, '.', '');
            echo 'snn_require(fmt::floating_point(-', $number, ', ', $decimals, ') == "', $s, '");';
            echo "\n";

            $s = number_format(-1.0 * $n, $decimals, ',', ' ');
            echo 'snn_require(fmt::floating_point(';
            echo '-', $number, ', ', $decimals, ", ',', ' ')";
            echo ' == "', $s, '");';
            echo "\n";

            // cstrview

            $s = number_format($n, $decimals, '٫', '٬');
            echo 'snn_require(fmt::floating_point(';
            echo $number, ', ', $decimals, ', "٫", "٬")';
            echo ' == "', $s, '");';
            echo "\n";

            $s = number_format(-1.0 * $n, $decimals, '٫', '٬');
            echo 'snn_require(fmt::floating_point(';
            echo '-', $number, ', ', $decimals, ', "٫", "٬")';
            echo ' == "', $s, '");';
            echo "\n";
        }
    }

    gen('0.0');
    gen('1.0');
    gen('1.2');
    gen('1.23');
    gen('12.34');
    gen('123.45');
    gen('1234.56');
    gen('12345.67');
    gen('123456.78');
    gen('1234567.89');
    gen('12345678.90');
    gen('123456789.01');
    gen('1234567890.12');

    gen('0.0001');
    gen('0.0012');
    gen('0.0123');
    gen('0.1234');
    gen('1.2345');
    gen('12.3456');
    gen('123.4567');
    gen('1234.5678');
    gen('12345.6789');
    gen('123456.7890');
    gen('1234567.8901');
    gen('12345678.9012');
    gen('123456789.0123');
    gen('1234567890.1234');

    gen('33.33');
    gen('33.3333');
    gen('3333.0');
    gen('3333.03333', 5);
    gen('3333.00333', 5);
    gen('3333.00033', 5);
    gen('3333.00003', 5);

    gen('99.99');
    gen('99.9999');
    gen('9999.0');
    gen('9999.09999', 5);
    gen('9999.00999', 5);
    gen('9999.00099', 5);
    gen('9999.00009', 5);

    gen('333333333333333.0', 0);
    gen('33333333333333.3', 1);
    gen('3333333333333.33', 2);
    gen('333333333333.333', 3);
    gen('33333333333.3333', 4);
    gen('3333333333.33333', 5);
    gen('333333333.333333', 6);
    gen('33333333.3333333', 7);
    gen('3333333.33333333', 7);
    gen('333333.333333333', 7);
    gen('33333.3333333333', 7);
    gen('3333.33333333333', 7);
    gen('333.333333333333', 7);
    gen('33.3333333333333', 7);
    gen('3.33333333333333', 7);

    gen('999999999999999.0', 0);
    gen('99999999999999.9', 1);
    gen('9999999999999.99', 2);
    gen('999999999999.999', 3);
    gen('99999999999.9999', 4);
    gen('9999999999.99999', 5);
    gen('999999999.999999', 6);
    gen('99999999.9999999', 7);
    gen('9999999.99999999', 7);
    gen('999999.999999999', 7);
    gen('99999.9999999999', 7);
    gen('9999.99999999999', 7);
    gen('999.999999999999', 7);
    gen('99.9999999999999', 7);
    gen('9.99999999999999', 7);

