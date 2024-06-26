# Введение в C# и платформу .Net
(Бакалавриат, 7-й семестр)

[Материалы курса.](https://sites.google.com/view/fitnsudotnet/%D0%B3%D0%BB%D0%B0%D0%B2%D0%BD%D0%B0%D1%8F-%D1%81%D1%82%D1%80%D0%B0%D0%BD%D0%B8%D1%86%D0%B0)

## Задача о привередливой принцессе.  

В некотором царстве, в некотором государстве жила была **принцесса** (Princess). Они жила в роскошном замке, носила изысканные наряды, 
в её саду росли самые прекрасные цветы. И всё потому, что принцесса была очень привередлива. И вот однажды пришла пора жениться.
Созвали со всего света царевичей, королевичей, тренеров по фитнессу и прочих прекрасных парней, и явилось **100 претендентов**
(Contender). 

![image](https://github.com/Regina-Kapralova/NSU-Labs/assets/115489350/63d94481-773e-47fe-9313-80b936619660)


Все эти претенденты образуют вполне упорядоченное множество по своим **личным качествам** (по степени хорошести (Mark)) и при этом
все они отличаются друг от друга. Претенденты толпятся в **прихожей** (Hall), и входят к принцессе, по одному. Причём их
порядок определён случайным образом, то есть вероятность появления претендента первым, вторым или
сотым совершенно одинакова.

После одного короткого свидания принцесса не может понять, насколько плох или хорош тот или иной кандидат. Но у
принцессы есть фрейлина, она же ее **подруга** (Freind). Подруга - девушка общительная, она знает всех 
претендентов и также тусит с ними в прихожей. Принцесса может позвать свою подругу и спросить, **кто лучше: тот
или этот**. Фрейлина ответит абсолютно объективно. Но она немного “себе на уме”, и потому даст ответ только в том
случае, **если оба претендента уже побывали у принцессы**.

**Во время свидания (возможно, посоветовавшись с подругой) принцесса должна сказать, готова ли выйти замуж за данного
претендента**. Вернуться к нему она уже не сможет. После того, как принцесса сделает выбор, она выйдет в
прихожую, посмотрит на всех претендентов и поймет, кого же она выбрала.

Цель принцессы – наиболее счастливо выйти замуж. **Уровень счастья** определяется так: за лучшего жениха принцесса
получает 100 баллов, за второго по хорошести – 99 и так далее до 51. Если выбранный кандидат на 50-м месте или ниже,
принцесса понимает, что вышла замуж за неудачника, получает 0 баллов счастья и страдает всю жизнь. Если принцесса никого не выбрала, 
она получает 10 баллов счастья.

## Этапы выполнения работы:

**1.** Консольное приложение.  
**2.** Подключение IoС Container.  
**3.** Покрытие тестами.  

