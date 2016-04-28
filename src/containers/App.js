import React, { Component, PropTypes } from 'react-native'
import { bindActionCreators } from 'redux';
import { connect } from 'react-redux';
import { Router } from 'react-native-router-flux';

import Chat from '../components/Chat';
import * as MessageActions from '../actions/message';

const RouterWithRedux = connect()(Router);

const mapStateToProps = state => {
   return {
      messages: state.message,
      conversation: state.conversation
   };
};
const mapDispatchToProps = dispatch => bindActionCreators(MessageActions, dispatch);

class App extends Component {
	render() {
		return (
			this.props.children
		)
	}
}

export default connect(mapStateToProps, mapDispatchToProps)(App);