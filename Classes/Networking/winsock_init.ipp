//
// detail/impl/winsock_init.ipp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef STDNET_DETAIL_IMPL_WINSOCK_INIT_IPP
#define STDNET_DETAIL_IMPL_WINSOCK_INIT_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include "config.hpp"

#if defined(STDNET_WINDOWS) || defined(__CYGWIN__)

#include "socket_types.hpp"
#include "winsock_init.hpp"
#include "push_options.hpp"


void winsock_init_base::startup(data& d,
    unsigned char major, unsigned char minor)
{
  if (::InterlockedIncrement(&d.init_count_) == 1)
  {
    WSADATA wsa_data;
    long result = ::WSAStartup(MAKEWORD(major, minor), &wsa_data);
    ::InterlockedExchange(&d.result_, result);
  }
}

void winsock_init_base::cleanup(data& d)
{
  if (::InterlockedDecrement(&d.init_count_) == 0)
  {
    ::WSACleanup();
  }
}

void winsock_init_base::throw_on_error(data& d)
{
  long result = ::InterlockedExchangeAdd(&d.result_, 0);
  if (result != 0)
  {

  }
}

#include "pop_options.hpp"

#endif // defined(STDNET_WINDOWS) || defined(__CYGWIN__)

#endif // STDNET_DETAIL_IMPL_WINSOCK_INIT_IPP
