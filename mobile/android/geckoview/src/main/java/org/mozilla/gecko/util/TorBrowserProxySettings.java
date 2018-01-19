package org.mozilla.gecko.util;

import java.net.InetSocketAddress;
import java.net.Proxy;

import ch.boye.httpclientandroidlib.HttpHost;

public class TorBrowserProxySettings {
    private static String TOR_SOCKS_PROXY_ADDRESS = "127.0.0.1";
    private static int TOR_SOCKS_PROXY_PORT = 9050;
    private static String TOR_HTTP_PROXY_ADDRESS = "127.0.0.1";
    private static int TOR_HTTP_PROXY_PORT = 8118;
    private static boolean isInitialized = false;

    public static void initialize(String shost, int sport, String hhost, int hport) {
        if (isInitialized) {
            return;
        }
        isInitialized = true;
        if (shost != null) {
            TOR_SOCKS_PROXY_ADDRESS = shost;
        }

        if (sport != -1) {
            TOR_SOCKS_PROXY_PORT = sport;
        }

        if (hhost != null) {
            TOR_SOCKS_PROXY_ADDRESS = shost;
        }

        if (hport != -1) {
            TOR_HTTP_PROXY_PORT = hport;
        }
    }

    public static Proxy getProxy() {
        return new Proxy(Proxy.Type.SOCKS, new InetSocketAddress(TOR_SOCKS_PROXY_ADDRESS, TOR_SOCKS_PROXY_PORT));
    }

    public static HttpHost getProxyHost() {
        return new HttpHost(TOR_HTTP_PROXY_ADDRESS, TOR_HTTP_PROXY_PORT);
    }
}
