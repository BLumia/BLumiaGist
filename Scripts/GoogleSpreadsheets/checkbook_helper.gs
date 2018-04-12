function resetBgColor() {
  SpreadsheetApp.getActiveSheet().getDataRange().setBackground(null);
}

function setDebtColor() {
  var sheet = SpreadsheetApp.getActiveSheet();
  var range = sheet.getDataRange();
  setRangeBackground(range);
}

function setRangeBackground(range) {
  var data = range.getValues();
  for (var i = 4; i <= 200; i++) { // index start at 1
    if (i > data.length) break;
    // debt row
    var cell = range.getCell(i, 3);
    var cellValue = cell.getValue();
    if (cellValue - 0 > 0.001) { // incoming
      if (cellValue > 61 && cellValue <= 261) cell.setBackground("#a0c238");
      else if (cellValue > 261) cell.setBackground("#0074bf"); 
      else cell.setBackground("#c4fdb3");
    } else if (cellValue - 0 < -0.001) { // outcoming
      if (cellValue < -61 && cellValue >= -261) cell.setBackground("#de9610");
      else if (cellValue < -261) cell.setBackground("#c93a40"); 
      else cell.setBackground("#fffacd");
    } else {
      cell.setBackground(null);
    }
  }
}

function doInit() {
  var sheet = SpreadsheetApp.getActiveSheet();
  var range = sheet.getDataRange();
  range.getCell(3, 1).setBackground("#A4C2F4");
  range.getCell(3, 2).setBackground("#A4C2F4");
  range.getCell(3, 3).setBackground("#A4C2F4");
  range.getCell(3, 4).setBackground("#A4C2F4");
  range.getCell(3, 5).setBackground("#A4C2F4");
}

/*
Sidebar stuff
*/

function onOpen() {
  SpreadsheetApp.getUi() 
      .createMenu('Checkbook Helper')
      .addItem('Show sidebar', 'showSidebar')
      .addToUi();
  showSidebar();
}

function showSidebar() {
  var html = HtmlService.createHtmlOutputFromFile('checkbook_sidebar')
      .setTitle('Checkbook Helper')
      .setWidth(300);
  SpreadsheetApp.getUi() 
      .showSidebar(html);
}

function fillActivedRowColor() {
  var activeRow = ActiveSheet.getActiveRange().getRow();
  setRangeBackground(activeRow);
}