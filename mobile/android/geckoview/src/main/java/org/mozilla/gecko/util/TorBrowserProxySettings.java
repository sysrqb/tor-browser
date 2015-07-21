package org.mozilla.gecko.util;

import android.content.SharedPreferences;
import android.util.Log;
import android.util.Pair;

import java.net.InetSocketAddress;
import java.net.Proxy;

import ch.boye.httpclientandroidlib.HttpHost;

public class TorBrowserProxySettings {
    private static final String LOGTAG = "TorBrowserProxySettings";

    // These are the sDefault Orbot values
    private static final String DEFAULT_SOCKS_PROXY_ADDRESS = "127.0.0.1";
    private static final int DEFAULT_SOCKS_PROXY_PORT = 9050;
    private static final String DEFAULT_HTTP_PROXY_ADDRESS = "127.0.0.1";
    private static final int DEFAULT_HTTP_PROXY_PORT = 8118;

    private static String sOrbotSocksProxyAddress = "";
    private static int sOrbotSocksProxyPort = -1;
    private static String sOrbotHttpProxyAddress = "";
    private static int sOrbotHttpProxyPort = -1;


    private static SharedPreferences sSharedPrefs;

    private static final String PREF_NETWORK_PROXY_PREFIX = "network.proxy.";
    private static final String PREF_NETWORK_PROXY_HTTP = PREF_NETWORK_PROXY_PREFIX + "http";
    private static final String PREF_NETWORK_PROXY_HTTP_PORT = PREF_NETWORK_PROXY_PREFIX + "http_port";
    private static final String PREF_NETWORK_PROXY_SOCKS = PREF_NETWORK_PROXY_PREFIX + "socks";
    private static final String PREF_NETWORK_PROXY_SOCKS_PORT = PREF_NETWORK_PROXY_PREFIX + "socks_port";

    // Whenever we receive an new STATUS_ON message from Orbot, we
    // update the address and port it is using.
    public static void configure(String shost, int sport, String hhost, int hport) {
        Log.d(LOGTAG, "New Orbot HTTP configure: host: '" + hhost + "', port: " + hport);
        Log.d(LOGTAG, "New Orbot SOCKS configure: host: '" + shost + "', port: " + sport);
        if (shost != null) {
            sOrbotSocksProxyAddress = shost;
        }

        if (sport != -1) {
            sOrbotSocksProxyPort = sport;
        }

        if (hhost != null) {
            sOrbotHttpProxyAddress = shost;
        }

        if (hport != -1) {
            sOrbotHttpProxyPort = hport;
        }
    }

    public static void setSharedPreferences(SharedPreferences aPrefs) {
        sSharedPrefs = aPrefs;
    }

    public static Proxy getProxy() {
        final Pair<String, Integer> config = getSOCKSConfiguredPref();

        String host = config.first;
        int port = config.second;

        if (host.equals("") || port == -1) {
            if (sOrbotSocksProxyAddress.equals("") || sOrbotSocksProxyPort == -1) {
                host = DEFAULT_SOCKS_PROXY_ADDRESS;
                port = DEFAULT_SOCKS_PROXY_PORT;
            } else {
                host = sOrbotSocksProxyAddress;
                port = sOrbotSocksProxyPort;
            }
        }

        Log.d(LOGTAG, "Returning SOCKS Proxy for '" + host + "':" + port);
        return new Proxy(Proxy.Type.SOCKS, new InetSocketAddress(host, port));
    }

    public static HttpHost getProxyHost() {
        final Pair<String, Integer> config = getHTTPConfiguredPref();

        String host = config.first;
        int port = config.second;

        if (host.equals("") || port == -1) {
            if (sOrbotHttpProxyAddress.equals("") || sOrbotHttpProxyPort == -1) {
                host = DEFAULT_HTTP_PROXY_ADDRESS;
                port = DEFAULT_HTTP_PROXY_PORT;
            } else {
                host = sOrbotHttpProxyAddress;
                port = sOrbotHttpProxyPort;
            }
        }
        Log.d(LOGTAG, "Returning HTTP Proxy for '" + host + "':" + port);
        return new HttpHost(host, port);
    }

    // Return a Pair containing the host string and port number prefs
    private static Pair<String, Integer> getHTTPConfiguredPref() {
        if (sSharedPrefs == null) {
            return new Pair<String, Integer>("", -1);
        }
        final String http_addr = sSharedPrefs.getString(PREF_NETWORK_PROXY_HTTP, "");
        final int http_port = sSharedPrefs.getInt(PREF_NETWORK_PROXY_HTTP_PORT, -1);

        return new Pair<String, Integer>(http_addr, http_port);
    }

    // Return a Pair containing the host string and port number prefs
    private static Pair<String, Integer> getSOCKSConfiguredPref() {
        if (sSharedPrefs == null) {
            return new Pair<String, Integer>("", -1);
        }
        final String socks_addr = sSharedPrefs.getString(PREF_NETWORK_PROXY_SOCKS, "");
        final int socks_port = sSharedPrefs.getInt(PREF_NETWORK_PROXY_SOCKS_PORT, -1);

        return new Pair<String, Integer>(socks_addr, socks_port);
    }
}
