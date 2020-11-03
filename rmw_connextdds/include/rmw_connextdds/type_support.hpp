/******************************************************************************
 *
 * (c) 2020 Copyright, Real-Time Innovations, Inc. (RTI)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef RMW_CONNEXT__TYPE_SUPPORT_HPP_
#define RMW_CONNEXT__TYPE_SUPPORT_HPP_

#include "rmw_connextdds/context.hpp"

#include "rosidl_typesupport_fastrtps_c/identifier.h"
#include "rosidl_typesupport_fastrtps_cpp/identifier.hpp"
#include "rosidl_typesupport_fastrtps_cpp/message_type_support.h"
#include "rosidl_typesupport_fastrtps_cpp/service_type_support.h"
#define RMW_FASTRTPS_CPP_TYPESUPPORT_C rosidl_typesupport_fastrtps_c__identifier
#define RMW_FASTRTPS_CPP_TYPESUPPORT_CPP rosidl_typesupport_fastrtps_cpp::typesupport_identifier

#if RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT

#include "rosidl_typesupport_introspection_cpp/field_types.hpp"
#include "rosidl_typesupport_introspection_cpp/identifier.hpp"
#include "rosidl_typesupport_introspection_cpp/message_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/service_introspection.hpp"
#include "rosidl_typesupport_introspection_cpp/visibility_control.h"

#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "rosidl_typesupport_introspection_c/service_introspection.h"
#include "rosidl_typesupport_introspection_c/visibility_control.h"

#endif /* RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT */

struct RMW_Connext_RequestReplyMessage
{
    bool request;
    rmw_gid_t gid;
    int64_t sn;
    void *payload;
};


struct RMW_Connext_Message
{
    const void *user_data;
    bool serialized;
};


class RMW_Connext_MessageTypeSupport
{
    const rosidl_message_type_support_t *_type_support_fastrtps;
    bool _unbounded;
    bool _empty;
    uint32_t _serialized_size_max;
    std::string _type_name;
    RMW_Connext_MessageType _message_type;

public:
    static const uint32_t ENCAPSULATION_HEADER_SIZE = 4;

    RMW_Connext_MessageTypeSupport(
        const RMW_Connext_MessageType message_type,
        const rosidl_message_type_support_t *const type_supports,
        const char *const type_name);
    
    // ~RMW_Connext_MessageTypeSupport()
    // {
    //     delete this->_type_support_fastrtps;
    // }

    const message_type_support_callbacks_t * callbacks_fastrtps()
    {
        return static_cast<const message_type_support_callbacks_t *>(
                        this->_type_support_fastrtps->data);
    }

    const char * type_name() const {
        return this->_type_name.c_str();
    }

    uint32_t type_serialized_size_max() const {
        return this->_serialized_size_max;
    }

    bool unbounded() const
    {
        return this->_unbounded;
    }

    bool empty() const
    {
        return this->_empty;
    }

    bool type_requestreply() const
    {
        return this->_message_type == RMW_CONNEXT_MESSAGE_REQUEST ||
                this->_message_type == RMW_CONNEXT_MESSAGE_REPLY;
    }

    bool type_userdata() const
    {
        return this->_message_type == RMW_CONNEXT_MESSAGE_USERDATA;
    }

    uint32_t serialized_size_max(
        const void *const ros_msg,
        const bool include_encapsulation = true);

    rmw_ret_t serialize(
        const void *const ros_msg,
        rcutils_uint8_array_t *const to_buffer);
    
    rmw_ret_t deserialize(
        void *const ros_msg,
        const rcutils_uint8_array_t *const from_buffer,
        size_t &size_out);

    static
    RMW_Connext_MessageTypeSupport*
    register_type_support(
        rmw_context_impl_t *const ctx,
        const rosidl_message_type_support_t *const type_supports,
        DDS_DomainParticipant *const participant,
        bool& registered,
        const RMW_Connext_MessageType message_type = RMW_CONNEXT_MESSAGE_USERDATA,
        const void *const intro_members = nullptr,
        const bool intro_members_cpp = false,
        std::string *const type_name = nullptr);
    
    static rmw_ret_t unregister_type_support(
        rmw_context_impl_t *const ctx,
        DDS_DomainParticipant *const participant,
        const char *const type_name);

    static const rosidl_message_type_support_t * get_type_support_fastrtps(
        const rosidl_message_type_support_t *const type_supports);

#if RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT
    static const rosidl_message_type_support_t * get_type_support_intro(
        const rosidl_message_type_support_t *const type_supports,
        bool &cpp_version);
#endif /* RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT */
    
    static void type_info(
        const rosidl_message_type_support_t *const type_support,
        uint32_t &serialized_size_max,
        bool &unbounded,
        bool &empty);
};


class RMW_Connext_ServiceTypeSupportWrapper
{
public:
    static
    const rosidl_service_type_support_t *
    get_type_support_fastrtps(
        const rosidl_service_type_support_t *const type_supports);
    
#if RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT
    static
    const rosidl_service_type_support_t *
    get_type_support_intro(
        const rosidl_service_type_support_t *const type_supports,
        bool &cpp_version);
#endif /* RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT */
    
    static
    const rosidl_message_type_support_t *
    get_request_type_support(
        const rosidl_service_type_support_t *const type_supports,
        const void **const svc_members_out,
        bool &svc_members_cpp)
    {
        const rosidl_service_type_support_t *const svc_type_support_fastrtps =  
            RMW_Connext_ServiceTypeSupportWrapper::get_type_support_fastrtps(
                type_supports);
        if (nullptr == svc_type_support_fastrtps)
        {
            return nullptr;
        }
        auto svc_callbacks =
            static_cast<const service_type_support_callbacks_t *>(
                svc_type_support_fastrtps->data);

#if RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT
        const rosidl_service_type_support_t *const svc_type_support_intro =  
            RMW_Connext_ServiceTypeSupportWrapper::get_type_support_intro(
                type_supports, svc_members_cpp);
        if (nullptr == svc_type_support_intro)
        {
            return nullptr;
        }

        if (svc_members_cpp)
        {
            const rosidl_typesupport_introspection_c__ServiceMembers *const svc_members =
                (const rosidl_typesupport_introspection_c__ServiceMembers *)svc_type_support_intro->data;
            
            *svc_members_out = svc_members->request_members_;
        }
        else
        {
            const rosidl_typesupport_introspection_cpp::ServiceMembers *const svc_members =
                (const rosidl_typesupport_introspection_cpp::ServiceMembers *)svc_type_support_intro->data;
            
            *svc_members_out = svc_members->request_members_;
        }
#else
    UNUSED_ARG(svc_members_out);
    UNUSED_ARG(svc_members_cpp);
#endif /* RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT */

        return svc_callbacks->request_members_;
    }

    static
    const rosidl_message_type_support_t *
    get_response_type_support(
        const rosidl_service_type_support_t *const type_supports,
        const void **const svc_members_out,
        bool &svc_members_cpp)
    {
        const rosidl_service_type_support_t *const svc_type_support_fastrtps =  
            RMW_Connext_ServiceTypeSupportWrapper::get_type_support_fastrtps(
                type_supports);
        if (svc_type_support_fastrtps == NULL)
        {
            // RMW_CONNEXT_LOG_ERROR("failed to lookup FastRTPS type support")
            return nullptr;
        }
        auto svc_callbacks =
            static_cast<const service_type_support_callbacks_t *>(
                svc_type_support_fastrtps->data);

#if RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT
        const rosidl_service_type_support_t *const svc_type_support_intro =  
            RMW_Connext_ServiceTypeSupportWrapper::get_type_support_intro(
                type_supports, svc_members_cpp);
        if (nullptr == svc_type_support_intro)
        {
            return nullptr;
        }

        if (svc_members_cpp)
        {
            const rosidl_typesupport_introspection_c__ServiceMembers *const svc_members =
                (const rosidl_typesupport_introspection_c__ServiceMembers *)svc_type_support_intro->data;
            
            *svc_members_out = svc_members->response_members_;
        }
        else
        {
            const rosidl_typesupport_introspection_cpp::ServiceMembers *const svc_members =
                (const rosidl_typesupport_introspection_cpp::ServiceMembers *)svc_type_support_intro->data;
            
            *svc_members_out = svc_members->response_members_;
        }
#else
        UNUSED_ARG(svc_members_out);
        UNUSED_ARG(svc_members_cpp);
#endif /* RMW_CONNEXT_HAVE_INTRO_TYPE_SUPPORT */

        return svc_callbacks->response_members_;
    }

    static std::string get_request_type_name(
        const rosidl_service_type_support_t *const type_supports);

    static std::string get_response_type_name(
        const rosidl_service_type_support_t *const type_supports);
};


#endif /* RMW_CONNEXT__TYPE_SUPPORT_HPP_ */