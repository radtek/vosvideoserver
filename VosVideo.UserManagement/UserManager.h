#pragma once
#include <set>
#include <mutex>
#include <ppltasks.h>
#include "VosVideo.Communication/CommunicationManager.h"
#include "VosVideo.Configuration/ConfigurationManager.h"
#include "LogInRequest.h"
#include "LogInResponse.h"


namespace vosvideo
{
	namespace usermanagement
	{
		class UserManager : public vosvideo::communication::MessageReceiver
		{
		public:
			UserManager(std::shared_ptr<vosvideo::communication::CommunicationManager> communicationManager, 
				        std::shared_ptr<vosvideo::configuration::ConfigurationManager> configurationManager,
						std::shared_ptr<vosvideo::communication::PubSubService> pubsubService);
			virtual ~UserManager();

			concurrency::task<LogInResponse> LogInAsync(LogInRequest const& loginRequest);

			virtual void OnMessageReceived(std::shared_ptr<vosvideo::data::ReceivedData> receivedMessage) override;

			std::wstring& GetAccountId();
			// Broadcast message to all connected peers 
			void NotifyAllUsers(std::shared_ptr<vosvideo::data::SendData>);

		private:
			bool logInInProgress_ = false;
			LogInRequest loginRequest_;
			concurrency::task_completion_event<LogInResponse> wsOpenedCompletionEvent_;
			LogInResponse logInResponse_;
			std::wstring userAccountId_;
			std::shared_ptr<vosvideo::communication::CommunicationManager> communicationManager_;
			std::shared_ptr<vosvideo::configuration::ConfigurationManager> configurationManager_;
			std::shared_ptr<vosvideo::communication::PubSubService> pubSubService_;
			std::set<std::wstring> clientPeers_;
			std::mutex mutex_;

			// List of peers, clients connected to RTBC via WebSocket
			void GetClientPeersFromJson(const web::json::value& jval, std::set<std::wstring>& clientPeers);
			// Single peer, clients connected to RTBC via WebSocket
			void GetClientPeerFromJson(const web::json::value& jval, std::set<std::wstring>& clientPeers);
			// Peers for WebSocket communication
			void GetTokenFromJson(web::json::value& jval, vosvideo::communication::Peer& p);
			void SetAccountIdFromUserJson( web::json::value& jval);
			std::wstring GetByKeyFromJson(web::json::value& jval, std::wstring key);
			void ReLoginAsync();
			void ReAuthAsync();
		};
	}
}
