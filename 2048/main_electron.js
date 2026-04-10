// Modules to control application life and create native browser window
const { app, BrowserWindow } = require('electron')
const path = require('node:path')
const util = require('util');
const addon = require('bindings')('g6-2048');
const prompt = require('prompt-sync')({ sigint: true });

function createWindow () {
  // Create the browser window.
  const mainWindow = new BrowserWindow({
    width: 800,
    height: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload_electron.js')
    }
  })

  // and load the index HTML of the app.
  mainWindow.loadFile('index_electron.html')

  // Open the DevTools.
  // mainWindow.webContents.openDevTools()
}

// This method will be called when Electron has finished
// initialization and is ready to create browser windows.
// Some APIs can only be used after this event occurs.
app.whenReady().then(() => {
  createWindow()

  app.on('activate', function () {
    // On macOS it's common to re-create a window in the app when the
    // dock icon is clicked and there are no other windows open.
    if (BrowserWindow.getAllWindows().length === 0) createWindow()
  })
})

// Quit when all windows are closed, except on macOS. There, it's common
// for applications and their menu bar to stay active until the user quits
// explicitly with Cmd + Q.
app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') app.quit()
})

// In this file you can include the rest of your app's specific main process
// code. You can also put them in separate files and require them here.
const http = require('http');

GAME_2048_STATUS_NOT_OVER = 0;
GAME_2048_STATUS_WON = 1;
GAME_2048_STATUS_LOST = 2;
var st = GAME_2048_STATUS_NOT_OVER;
var mat = null;
const server = http.createServer((req, res) => {
    //TODO: is it necessary to consider multiple clients accessing
    if(req.url == '/init') {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        st = GAME_2048_STATUS_NOT_OVER;
        mat = addon.init();
        addon.add(mat);
        dat = addon.peek(mat);
        res.end(JSON.stringify({mat:JSON.stringify(dat), st: JSON.stringify(st)}));
    }
    else if(req.url == '/up') {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        mat = addon.up(mat);
        st = addon.get(mat);
        if(st == GAME_2048_STATUS_NOT_OVER) {
            addon.add(mat);
        }
        dat = addon.peek(mat);
        res.end(JSON.stringify({mat:JSON.stringify(dat), st: JSON.stringify(st)}));
    }
    else if(req.url == '/left') {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        mat = addon.left(mat);
        st = addon.get(mat);
        if(st == GAME_2048_STATUS_NOT_OVER) {
            addon.add(mat);
        }
        dat = addon.peek(mat);
        res.end(JSON.stringify({mat:JSON.stringify(dat), st: JSON.stringify(st)}));
    }
    else if(req.url == '/right') {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        mat = addon.right(mat);
        st = addon.get(mat);
        if(st == GAME_2048_STATUS_NOT_OVER) {
            addon.add(mat);
        }
        dat = addon.peek(mat);
        res.end(JSON.stringify({mat:JSON.stringify(dat), st: JSON.stringify(st)}));
    }
    else if(req.url == '/down') {
        res.statusCode = 200;
        res.setHeader('Content-Type', 'application/json');
        mat = addon.down(mat);
        st = addon.get(mat);
        if(st == GAME_2048_STATUS_NOT_OVER) {
            addon.add(mat);
        }
        dat = addon.peek(mat);
        res.end(JSON.stringify({mat:JSON.stringify(dat), st: JSON.stringify(st)}));
    }
    else {
        console.log('unknown req: ' + req.url);
    }
});

server.listen(9527, () => {
    console.log(
        'Server running at http://localhost:9527/'
    );
});

