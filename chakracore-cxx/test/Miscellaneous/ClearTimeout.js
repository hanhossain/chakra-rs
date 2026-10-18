WScript.SetTimeout(function() {
    WScript.Echo("one");
    WScript.ClearTimeout(two);
    WScript.ClearTimeout(outOfOrder);
}, 10);

var two = WScript.SetTimeout(function() {
    WScript.Echo("two");
}, 50);

WScript.SetTimeout(function() {
    WScript.Echo("three");
    WScript.ClearTimeout(insane);
}, 100);

var insane = WScript.SetTimeout(() => {
    WScript.Echo("four: this should be cancelled");
}, 5000);

var outOfOrder = WScript.SetTimeout(() => {
    WScript.Echo("five: this should be cancelled");
}, 1000);
