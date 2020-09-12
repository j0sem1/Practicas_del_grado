FOR %%s IN (0,1) DO (

	FOR %%p IN (100,150) DO (

		FOR %%c IN (8,85,9,95) DO (

			FOR %%m IN (05,1,125,15,175) DO ( 

				Sudoku_CasosPrueba.exe Caso-A1.txt %%p 12000 0.%%c 0.%%m %%s
				Sudoku_CasosPrueba.exe Caso-A2.txt %%p 12000 0.%%c 0.%%m %%s
				Sudoku_CasosPrueba.exe Caso-A3.txt %%p 12000 0.%%c 0.%%m %%s
				Sudoku_CasosPrueba.exe Caso-A4.txt %%p 12000 0.%%c 0.%%m %%s
				Sudoku_CasosPrueba.exe Caso-A5.txt %%p 12000 0.%%c 0.%%m %%s

			)

		)

	)

)