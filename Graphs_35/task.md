# Задача 35. Стрельба
	Имя входного файла: input.txt
	Имя выходного файла: output.txt
	Ограничение по времени: 1 с
	Ограничение по памяти: нет
На соревнованиях по стрельбе каждый участник будет стрелять в цель, которая представляет собой прямоугольник, разделённый на квадраты. Цель содержит r ⋅ c квадратов, расположенных в r строках и c столбцах. Квадраты выкрашены в белый или чёрный цвет. В каждом столбце находится ровно 2 белых и r − 2 чёрных квадрата. Строки пронумерованы от 1 до r сверху вниз, а столбцы — от 1 до c слева направо. Стрелок имеет ровно c стрел. Последовательность из c выстрелов называется корректной, если в каждом столбце поражён ровно один белый квадрат, а в каждой строке — не менее одного белого квадрата.
Необходимо проверить, существует ли корректная последовательность выстрелов, и если да, то найти одну из них.
### Формат входных данных
Первая строка содержит два целых числа r и c (2 ≤ r ≤ c ≤ 300 000), разделённых пробелом,— число строк и столбцов.
Каждая из следующих c строк в блоке содержит два натуральных числа, разделённых пробелом. Числа в i + 1-й строке определяют номера строк, где расположены белые квадраты в i-м столбце.
### Формат выходных данных
Выведите последовательность из c номеров строк (разделённых пробелом) корректной последовательности выстрелов в белые клетки столбцов 1, 2, …, c соответственно или сообщение No, если такой последовательности не существует.
### Пример
|input.txt|output.txt|
|--|--|
|4 4<br>2 4<br>3 4<br>1 3<br>1 4|2 3 1 4|