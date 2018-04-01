function resetBgColor() {
  SpreadsheetApp.getActiveSheet().getDataRange().setBackground(null);
}

function setDebtColor() {
  var sheet = SpreadsheetApp.getActiveSheet();
  var range = sheet.getDataRange();
  var data = range.getValues();
  for (var i = 4; i <= 200; i++) { // index start at 1
    if (i > data.length) break;
    // debt row
    var cell = range.getCell(i, 3);
    var cellValue = cell.getValue();
    if (cellValue - 0 > 0.001) { // incoming
      if (cellValue > 61) cell.setBackground("#a0c238");
      else if (cellValue > 261) cell.setBackground("#0074bf"); 
      else cell.setBackground("#c4fdb3");
    } else if (cellValue - 0 < 0.001) { // outcoming
      if (cellValue < -61) cell.setBackground("#de9610");
      else if (cellValue < -261) cell.setBackground("#c93a40"); 
      else cell.setBackground("#fffacd");
    } else {
      cell.setBackground(null);
    }
  }
}
