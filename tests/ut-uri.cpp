#include <gtest/gtest.h>
#include "../includes/http/uri.h"

TEST(ProxyUriRedirection, test1) {
    auto uri = Uri("/bpc");
    uri.buildProxyRedirectionUri("bpc");
    EXPECT_EQ(uri.getProxyRedirectionUri(), "/");
}

TEST(ProxyUriRedirection, test2) {
    auto uri = Uri("/bpc/");
    uri.buildProxyRedirectionUri("bpc");
    EXPECT_EQ(uri.getProxyRedirectionUri(), "/");
}

TEST(ProxyUriRedirection, test3) {
    auto uri = Uri("/bpc/greet");
    uri.buildProxyRedirectionUri("bpc");
    EXPECT_EQ(uri.getProxyRedirectionUri(), "/greet");
}

TEST(ProxyUriRedirection, test4) {
    auto uri = Uri("/bpc/greet/jordan");
    uri.buildProxyRedirectionUri("bpc");
    EXPECT_EQ(uri.getProxyRedirectionUri(), "/greet/jordan");
}