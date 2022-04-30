//Load natives
let N;
try {
    N = require('./native/build/Release/native')
} catch(e) {
    throw "Failed to load native code, did you run setup.bat to compile?"
}

const FLAG = {
    TYPE:{
        KEYBOARD:       N.TYPE_KEYBOARD,
        MOUSE:          N.TYPE_MOUSE
    },
    KEY:{
        EXTENDEDKEY:    N.KFLAG_EXTENDEDKEY,
        KEYUP:          N.KFLAG_KEYUP,
        UNICODE:        N.KFLAG_UNICODE,
        SCANCODE:       N.KFLAG_SCANCODE
    },
    MOUSE:{
        MOVE:           N.MFLAG_MOVE,
        ABSOLUTE:       N.MFLAG_ABSOLUTE,
        LEFTDOWN:       N.MFLAG_LEFTDOWN,
        LEFTUP:         N.MFLAG_LEFTUP,
        RIGHTDOWN:      N.MFLAG_RIGHTDOWN,
        RIGHTUP:        N.MFLAG_RIGHTUP,
        MIDDLEDOWN:     N.MFLAG_MIDDLEDOWN,
        MIDDLEUP:       N.MFLAG_MIDDLEUP
    }
}

function keyEvent(scancode, extraFlags=0){
    return {type: FLAG.TYPE.KEYBOARD, scancode, flags: (FLAG.KEY.SCANCODE | extraFlags)}
}

const KEYUP = FLAG.KEY.KEYUP;
const keyevents = {
    "CTRL+V":[
        keyEvent(29),keyEvent(47),
        keyEvent(47, KEYUP), keyEvent(29, KEYUP)
    ],
    "CTRL+A":[
        [keyEvent(29),
        keyEvent(30)],
        [keyEvent(30, KEYUP),
        keyEvent(29, KEYUP)]
    ],
    "CTRL+TAB":[
        keyEvent(29),
        keyEvent(15),
        keyEvent(15, KEYUP),
        keyEvent(29, KEYUP)
    ],
    "ENTER":[
        keyEvent(28),
        keyEvent(28, KEYUP)
    ],
    "DOWN+DOWN":[
        keyEvent(80, FLAG.KEY.EXTENDEDKEY),
        keyEvent(80, FLAG.KEY.EXTENDEDKEY | KEYUP),
        keyEvent(80, FLAG.KEY.EXTENDEDKEY),
        keyEvent(80, FLAG.KEY.EXTENDEDKEY | KEYUP),
    ],
    "BACKSPACE":[
        keyEvent(14),
        keyEvent(14, KEYUP)
    ]
};

let eventQueue = [];

function schedule(event){
    eventQueue.push(event);
}

function schedule_key(info){
    schedule({type:"key", info});
}

function schedule_clip(info){
    schedule({type:"clip", info});
}

function schedule_custom(callback){
    schedule({type:"custom", callback});
}

function schedule_move(info){
    schedule({type:"move", info});
}

module.exports = {
    native:{
        setClipboard:   N.setClipboard,
    
        sendInputs:      N.sendInputs,
        getKeyState:    N.getKeyState,
        getScanCode:    N.getScanCode,

        getCursorPos:   N.getCursorPos,
        setCursorPos:   N.setCursorPos,
    
        FLAG
    },
    keyEvent, keyevents,

    schedule, schedule_key, schedule_clip, schedule_move, schedule_custom,

    clearSchedule(){
        eventQueue = [];
    },

    getQueuesize(){
        return eventQueue.length;
    },
    
    handleEvent:()=>{
        let rv = 0;
        if(eventQueue.length>0){
            let event = eventQueue.shift();
            //console.log("Handling event", event);
            switch(event.type){
                case "key" : 
                    if(Array.isArray(event.info)){
                        rv = N.sendInputs(event.info); 
                    } else {
                        rv = N.sendInputs([event.info]); 
                    }
                    break;
                case "clip": rv = N.setClipboard(event.info); break;
                case "move": rv = N.setCursorPos(event.info.x, event.info.y); break;
                case "custom": rv = 1; event.callback(); break;
                default:
                    console.error("Skipped an event!");
                    rv=1;
            }
        } else {
            rv = 1;
        }

        //if the next event is a custom one, execute it with this current one (slight speed save)
        if(eventQueue.length>0 && eventQueue[0].type=="custom") eventQueue.shift().callback();

        return rv;
    },

    sendString:(string)=>{
        schedule_clip(string);
        //keyevents["CTRL+V"].forEach(schedule_key);
        schedule_key(keyevents["CTRL+V"]);
    },

    backspace:(number=1)=>{
        for(let i=0; i<number; i++){
            keyevents["BACKSPACE"].forEach(schedule_key);
        }
    },
    
    nextLine:(ssc=false)=>{
        let eventSet = keyevents[ssc?"DOWN+DOWN":"ENTER"];
        eventSet.forEach(schedule_key);
    },

    nextCell:()=>{
        schedule_key(keyevents["CTRL+TAB"])//.forEach(schedule_key);
    },

    selectLine:(ssc=true)=>{
        if(!ssc) throw "not implemented";
        keyevents["CTRL+A"].forEach(schedule_key);
    },

    click:()=>{
        [
            {
                type: FLAG.TYPE.MOUSE, 
                flags: FLAG.MOUSE.MOVE,
                dx:20,
                dy:0
            },{
                type: FLAG.TYPE.MOUSE, 
                flags: FLAG.MOUSE.MOVE,
                dx:-20,
                dy:0
            },[{
                type: FLAG.TYPE.MOUSE, 
                flags: FLAG.MOUSE.LEFTDOWN
            },
            {
                type: FLAG.TYPE.MOUSE, 
                flags: FLAG.MOUSE.LEFTUP
            }
        ]].forEach(schedule_key)
    },

    getCursorPos:()=>{
        let {x, y, success} = N.getCursorPos();
        if(!success) throw "Failed to get cursor position";
        return {x, y}
    },

    moveTo:({x, y})=>{
        let success = N.setCursorPos(x,y);
        if(!success) throw "Failed to set cursor position";
    },

    interpolate(a, b, fraction){
        let x = a.x + (b.x-a.x)*fraction;
        let y = a.y + (b.y-a.y)*fraction;
        return {x,y}
    }

}