A native node addon for obtaining system idle time in miliseconds and current active window.

## Usage

Make sure node-gyp is installed globally: `npm install node-gyp -g`

Checkout this Repo and run `npm install`

Run `npm run rebuild` after making changes to C++ code.

Run `npm run build` to build x64 version for Electron.

See example in test.js. Run with `node test.js`

## Referrences

Base on the following resources:

* https://github.com/paulcbetts/node-system-idle-time
* http://stackoverflow.com/questions/10740143/get-current-active-window-title-in-c
* http://suanfazu.com/t/get-the-title-of-the-current-active-window-document-in-mac-os-x/9485/3
