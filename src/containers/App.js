import React, { Component } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';

import Bubble from '../components/Bubble';
import Chat from '../components/Chat';
import ChatCell from '../components/ChatCell';
import AnimatedTextInput from '../components/AnimatedTextInput';

import * as CounterActions from '../actions/counter';
import * as MessageActions from '../actions/message';

console.log(MessageActions);

@connect(
  state => ({
    counter: state.counter,   
  })
)
export default class App extends Component {	
	render() {
		const actions = bindActionCreators(MessageActions, this.props.dispatch)
		return <Chat {...actions} />
	}
}