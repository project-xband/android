# "Project xBand" Android client
[![Build Status](https://travis-ci.org/jhen0409/react-native-boilerplate.svg)](https://travis-ci.org/jhen0409/react-native-boilerplate) [![Dependency Status](https://david-dm.org/jhen0409/react-native-boilerplate.svg)](https://david-dm.org/jhen0409/react-native-boilerplate) [![devDependency Status](https://david-dm.org/jhen0409/react-native-boilerplate/dev-status.svg)](https://david-dm.org/jhen0409/react-native-boilerplate#info=devDependencies)

## Installation

See [Getting Started](https://facebook.github.io/react-native/docs/getting-started.html) to install requirement tools.

```bash
$ npm install -g react-native-cli
$ npm install
```

## Development

#### Android (5.0+)

Open Android emulator (recommented [Genymotion](https://www.genymotion.com)) and run command: (Or connect real device via USB)

```bash
$ npm run reverse  # react-native local server, remotedev local server
$ npm run android
```

__*[Emulator only]*__ If you're still use Android 4.0, it will cannot use `adb reverse`, you should use `10.0.2.2` (Genymotion: `10.0.3.2`) instead of `localhost` in [configureStore.js](src/configureStore.js#L15).

## Contribute

If you want to contribute, skimm the source, look for the "TODO"s and submit a pull request

We are plan on adding
 - Analitics (some redux middleware?) [ ]
 - Immutable.js (datomic <3) [ ]
 - VOIP [ ]
 - seach messages [ ]
 - send voice messages [ ]
 - contacts [ ]
 - send new message through material button [ ]
 - edit your profile - profile view [ ]
 - deleting, editing and flagging content in the app (messages, conversations, etc) [ ]
 - fixing all the todos in code [ ]
 - establish consistent linting (ESLint) [ ]
 - focus on performance of the app - eliminate uneccesairy libs [ ]
 - add security, encryption [ ]
 - make use redux remote dev tools [ ]
 - make a secrets file with the address of user, configs maybe,
   things that are good to have globaly but too important -> need to be secured


## Test

We used [react-native-mock](https://github.com/lelandrichardson/react-native-mock), and test with [Mocha](https://github.com/mochajs/mocha), [Enzyme](https://github.com/airbnb/enzyme).

```bash
$ npm test
```

## Credits

* [react-native-counter-ios-android](https://github.com/chentsulin/react-native-counter-ios-android)'s counter example.
* [react-native-boilerplate]() for providing us with lean and functioning starter code.

## LICENSE

[MIT](LICENSE)
