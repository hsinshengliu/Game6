/**
 * This file is loaded via the <script> tag in the index HTML file and will
 * be executed in the renderer process for that window. No Node.js APIs are
 * available in this process because `nodeIntegration` is turned off and
 * `contextIsolation` is turned on. Use the contextBridge API in `preload.js`
 * to expose Node.js functionality from the main process.
 */
const URL = 'http://localhost:9527';
const GAME_2048_STATUS_NOT_OVER = 0;
const GAME_2048_STATUS_WON = 1;
const GAME_2048_STATUS_LOST = 2;
function display(mat, st) {
    for(let i = 0; i < 4; i++) {
        for(let j = 0; j < 4; j++) {
            id = 'c' + (i * 4 + j);
            $('#'+id).text(mat[i][j]);
        }
    }
    var s = null;
    var sc = null;
    var frozen = false;
    switch(st) {
        case GAME_2048_STATUS_WON:
            s = 'won';
            sc = 'greenyellow';
            frozen = true;
            break;
        case GAME_2048_STATUS_LOST:
            s = 'lost';
            sc = 'firebrick';
            frozen = true;
            break;
        case GAME_2048_STATUS_NOT_OVER:
        default:
            s = 'keep going';
            sc = 'gold';
            frozen = false;
            break;
    }
    $('#'+'status').text(s);
    $('#'+'status').css('color', sc);
    if(frozen == true) {
        const movements = ['move-up', 'move-left', 'move-right', 'move-down'];
        $.each(movements, function(index, value) {
            $('#'+value).prop('disabled', true);
            $('#'+value).off('mousedown');
        });
    }
}

$(document).ready(function() {
    var req = '/init';
    $.ajax({
        url: URL + req,
        type: 'GET',
        success: function (data) {
            console.log(data);
            const m = JSON.parse(data['mat']);
            const s = JSON.parse(data['st']);
            display(m, s);
        },
        error: function (error) {
            console.error('Error:', error);
        }
    });
});

$(document).keydown(function(e) {
    switch(e.which) {
        case 38: //Up arrow
            console.log('up');
            $('#'+'move-up').mousedown();
            break;
        case 37: //Left arrow
            console.log('left');
            $('#'+'move-left').mousedown();
            break;
        case 39: //Right arrow
            console.log('right');
            $('#'+'move-right').mousedown();
            break;
        case 40: //Down arrow
            console.log('down');
            $('#'+'move-down').mousedown();
            break;
        default:
            console.log('omitted');
            break;
    }
    e.preventDefault();
});

$('button').bind('mousedown', function(){
    var req = null;
    var mv = false;
    switch(this.id) {
        case 'move-up':
            req = '/up';
            mv = true;
            break;
        case 'move-left':
            req = '/left';
            mv = true;
            break;
        case 'move-right':
            req = '/right';
            mv = true;
            break;
        case 'move-down':
            req = '/down';
            mv = true;
            break;
        default:
            break;
    }
    if(mv == true) {
        $.ajax({
            url: URL + req,
            type: 'GET',
            success: function (data) {
                console.log(data);
                const m = JSON.parse(data['mat']);
                const s = JSON.parse(data['st']);
                display(m, s);
            },
            error: function (error) {
                console.error('Error:', error);
            }
        });
    }
});

