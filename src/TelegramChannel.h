//
// Created by Simone on 14/09/22.
//

#ifndef ELOQUENTSURVEILLANCE_TELEGRAMCHANNEL_H
#define ELOQUENTSURVEILLANCE_TELEGRAMCHANNEL_H


#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <AsyncTelegram2.h>
#include "./globals.h"
#include "./traits/HasErrorMessage.h"


namespace EloquentSurveillance {
    /**
     * Send notifications via Telegram
     */
    class TelegramChannel : public HasErrorMessage {
    public:
        AsyncTelegram2 bot;

        /**
         *
         * @param bot
         */
        TelegramChannel(const char *token) :
                bot(_client),
                _token(token),
                _isInitialized(false) {

        }

        /**
         * Init bot
         *
         * @return
         */
        bool begin() {
            if (_isInitialized)
                return true;

            bot.setTelegramToken(_token);
            _client.setCACert(telegram_cert);

            if (!bot.begin())
                return setErrorMessage("Cannot init bot");

            return (_isInitialized = true);
        }

        /**
         * Send text message
         *
         * @param chatId
         * @param message
         * @return
         */
        bool sendMessage(const int64_t chatId, String message) {
            if (!begin())
                return false;

            TBMessage msg;
            msg.chatId = chatId;

            if (bot.sendMessage(msg, message.c_str(), nullptr, true)) {
                _client.stop();

                return setErrorMessage("");
            }

            _client.stop();

            return setErrorMessage("Send message error");
        }

        /**
         * Send photo as attachment
         *
         * @param chatId
         * @return
         */
        bool sendPhoto(const int64_t chatId) {
            if (!gIsFrame())
                return setErrorMessage("Cannot find frame");

            if (!begin())
                return false;

            bool response = bot.sendPhoto(chatId, gFrame->buf, gFrame->len);
            _client.stop();

            return response;
        }

    protected:
        bool _isInitialized;
        const char *_token;
        WiFiClientSecure _client;
    };
}

#endif //ELOQUENTSURVEILLANCE_TELEGRAMCHANNEL_H
