#ifndef __NEXT_NINEO_OBSERVER_DEFINE__
#define __NEXT_NINEO_OBSERVER_DEFINE__

#include <wx/wx.h>
#include <vector>

namespace NiUtils
{
    class ObClient;
    class ObServer;

    class ObClient
    {
        protected:
            ObServer* m_server;
            friend class ObServer;
            virtual void ObUpdate ( ObServer& server, const int& uid = 0 ) = 0;
        public:
            ObClient ();
            ~ObClient ();
    };
    
    class ObServer
    {
        private:
            typedef std::vector <ObClient*> ObClientList;
            ObClientList m_clientlist;
            bool HasClient ( ObClient* client );
        protected:
            void ObUpdate ( const int& uid = 0 );
        public:
            ObServer ();
            ~ObServer ();
            ObServer& operator+= ( ObClient* client );
            ObServer& operator-= ( ObClient* client );
    };
}
#endif //


