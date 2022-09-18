// Copyright (c) 2022 Mikael Simonsson <https://mikaelsimonsson.com>.
// SPDX-License-Identifier: BSL-1.0

package main

import "fmt"
import "time"

func main() {
	for year := 1990; year < 2030; year++ {
		for day := 1; day <= 4; day++ {
			y, w := time.Date(year, time.January, day, 0, 0, 0, 0, time.UTC).ISOWeek()
			fmt.Printf("snn_require(time::point{%d, 1, %d}.week()", year, day)
			fmt.Printf(" == time::year_week{%d, %d});\n", y, w)
		}

		for day := 28; day <= 31; day++ {
			y, w := time.Date(year, time.December, day, 0, 0, 0, 0, time.UTC).ISOWeek()
			fmt.Printf("snn_require(time::point{%d, 12, %d}.week()", year, day)
			fmt.Printf(" == time::year_week{%d, %d});\n", y, w)
		}
	}
}
