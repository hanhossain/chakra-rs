WScript.SetTimeout(function() {
    WScript.Echo("one");
    WScript.ClearTimeout(two);
}, 10);

var two = WScript.SetTimeout(function() {
    WScript.Echo("two");
}, 50);

WScript.SetTimeout(function() {
    WScript.Echo("three");
}, 100);
