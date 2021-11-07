'use strict';
const { google } = require('googleapis');


let sheets;

module.exports.submit = async (event, context, callback) => {

 const auth = authorize();
 sheets = google.sheets({ version: 'v4', auth });
 writeToSheet();
   const response = {
    statusCode: 200,
    body: JSON.stringify({
      message: 'Event registered successfully',
      input: event,
    }),
  };
  callback(null, response);
};

function authorize() {
  const oAuth2Client = new google.auth.GoogleAuth({
    credentials: {
      client_email: process.env.GOOGLE_SERVICE_ACCOUNT_EMAIL,
      private_key: process.env.GOOGLE_PRIVATE_KEY.replace(/\\n/gm, '\n')
    },
    scopes: ['https://www.googleapis.com/auth/spreadsheets'],
  });
  return oAuth2Client;
} 


async function writeToSheet() {

  let dateTime = new Date().toLocaleString("es-MX", { timeZone: 'America/Mexico_City'});

  return sheets.spreadsheets.values.append({
    spreadsheetId: process.env.GOOGLE_SPREADSHEET_ID,
    range: 'data',
    resource: {
      values: [dateTime.split(' ')]
    },
    insertDataOption: 'INSERT_ROWS',
    valueInputOption: 'RAW'
  });
}

