### Класс шаблонного датчика

Принимает запросы вида:
* [GetValue] [identifier]

Отправляет ответы вида:
* [Ok] [value] [timestamp]
* [Error] [lastError]

<table>
  <tr>
    <th>Поле</th>
    <th>Тип</th>
    <th>Описание</th>
  </tr>
  <tr>
    <td>[GetValue], [Ok], [Error]</td>
    <td>quint8</td>
    <td>Тип команды (значения перечисления CommandType)</td>
  </tr>
  <tr>
    <td>[identifier]</td>
    <td>QString</td>
    <td>Идентификатор датчика (из базы данных)</td>
  </tr>
  <tr>
    <td>[value]</td>
    <td>double</td>
    <td>Значение датчика в момент измерения (может иметь значение NAN, если при получении значения датчика произошла ошибка)</td>
  </tr>
  <tr>
    <td>[timestamp]</td>
    <td>QString</td>
    <td>Дата и время измерения значения датчика</td>
  </tr>
  <tr>
    <td>[lastError]</td>
    <td>QString</td>
    <td>Текст сообщения об ошибке при получении значения датчика</td>
  </tr>
</table>
