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
			<RouterWithRedux>
				<Scene key='root'>
					<Scene key="chat" component={Chat} title="Chat"/>
				</Scene>
			</RouterWithRedux>
		)
	}
	// <Chat {...this.props}/>
}

export default connect(mapStateToProps, mapDispatchToProps)(App);