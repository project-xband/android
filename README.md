# The "Project xBand" Android client [![Build Status](https://travis-ci.org/jhen0409/react-native-boilerplate.svg)](https://travis-ci.org/jhen0409/react-native-boilerplate) [![Dependency Status](https://david-dm.org/jhen0409/react-native-boilerplate.svg)](https://david-dm.org/jhen0409/react-native-boilerplate) [![devDependency Status](https://david-dm.org/jhen0409/react-native-boilerplate/dev-status.svg)](https://david-dm.org/jhen0409/react-native-boilerplate#info=devDependencies)

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
