<?php

    // Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
    // SPDX-License-Identifier: BSL-1.0

    if (!date_default_timezone_set('UTC'))
    {
        echo "Failed to set UTC timezone.\n";
        exit(1);
    }

    function gen($year, $month, $day)
    {
        $timestamp = gmmktime(0, 0, 0, $month, $day, $year);

        echo 'snn_require(time::point{', $year, ', ', $month, ', ', $day, '}.week()';
        echo ' == time::year_week{';
        echo date('o', $timestamp), ', ', ltrim(date('W', $timestamp), '0');
        echo '});', "\n";
    }

    for ($year = 1990; $year < 2030; ++$year)
    {
        for ($day = 1; $day <= 4; ++$day)
        {
            gen($year, 1, $day);
        }

        for ($day = 28; $day <= 31; ++$day)
        {
            gen($year, 12, $day);
        }
    }
