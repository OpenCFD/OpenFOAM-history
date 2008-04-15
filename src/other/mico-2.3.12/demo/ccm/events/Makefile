
all: all_target

DIR_PREFIX=../
include ../../MakeVars

INSTALL_DIR     = ccm/events
INSTALL_SRCS    = README Makefile channel.idl channel_impl.cc \
	client.cc consumer.idl consumer_impl.cc message.idl \
	message_impl.cc message_impl.h producer.idl \
	producer_impl.cc
INSTALL_SCRIPTS = events

all_target: .depend channel.$(SOEXT) consumer.$(SOEXT) producer.$(SOEXT) client

channel.$(SOEXT): channel_impl.pic.o channel_ccm.pic.o channel.pic.o \
		message_impl.pic.o message.pic.o
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(LDFLAGS) channel_impl.pic.o channel_ccm.pic.o channel.pic.o \
		message_impl.pic.o message.pic.o \
		-lmicoccm$(VERSION) -lmico$(VERSION) $(LDLIBS)
endif
	$(RM) $@
	$(LDSO) $(CXXFLAGS) $(LDFLAGS) -o channel \
		channel_impl.pic.o channel_ccm.pic.o channel.pic.o \
		message_impl.pic.o message.pic.o \
		-lmicoccm$(VERSION) -lmico$(VERSION) $(LDLIBS)

consumer.$(SOEXT): consumer_impl.pic.o consumer_ccm.pic.o consumer.pic.o \
		message_impl.pic.o message.pic.o
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(LDFLAGS) consumer_impl.pic.o consumer_ccm.pic.o consumer.pic.o \
		message_impl.pic.o message.pic.o \
		-lmicoccm$(VERSION) -lmico$(VERSION) $(LDLIBS)
endif
	$(RM) $@
	$(LDSO) $(CXXFLAGS) $(LDFLAGS) -o consumer \
		consumer_impl.pic.o consumer_ccm.pic.o consumer.pic.o \
		message_impl.pic.o message.pic.o \
		-lmicoccm$(VERSION) -lmico$(VERSION) $(LDLIBS)

producer.$(SOEXT): producer_impl.pic.o producer_ccm.pic.o producer.pic.o \
		message_impl.pic.o message.pic.o
ifeq ($(HAVE_PRELINK), yes)
	$(CXX) $(PRELINK_FLAG) $(LDFLAGS) producer_impl.pic.o producer_ccm.pic.o producer.pic.o \
		message_impl.pic.o message.pic.o \
		-lmicoccm$(VERSION) -lmico$(VERSION) $(LDLIBS)
endif
	$(RM) $@
	$(LDSO) $(CXXFLAGS) $(LDFLAGS) -o producer \
		producer_impl.pic.o producer_ccm.pic.o producer.pic.o \
		message_impl.pic.o message.pic.o \
		-lmicoccm$(VERSION) -lmico$(VERSION) $(LDLIBS)

client: client.o channel.pic.o consumer.pic.o producer.pic.o \
		message_impl.pic.o message.pic.o
	$(LD) $(CXXFLAGS) $(LDFLAGS) -o $@ client.o channel.pic.o \
		consumer.pic.o producer.pic.o message_impl.pic.o \
		message.pic.o -lmicoccm$(VERSION) -lmicocoss$(VERSION) \
		-lmico$(VERSION) $(LDLIBS)

run:
	events

clean:
	$(RM) channel.$(SOEXT) consumer.$(SOEXT) producer.$(SOEXT) client
	$(RM) channel_ccm.cc channel_ccm.h channel.cc channel.h
	$(RM) consumer_ccm.cc consumer_ccm.h consumer.cc consumer.h
	$(RM) producer_ccm.cc producer_ccm.h producer.cc producer.h
	$(RM) message.cc message.h
	$(RM) *.o *.$(SOEXT) *~ core *.ior .depend

channel_impl.pic.o: channel_impl.cc channel.h message_impl.h message.h
consumer_impl.pic.o: consumer_impl.cc consumer.h message_impl.h message.h
producer_impl.pic.o: producer_impl.cc producer.h message_impl.h message.h
message_impl.pic.o: message_impl.cc message_impl.h message.h
consumer_ccm.pic.o: consumer_ccm.cc
producer_ccm.pic.o: producer_ccm.cc
consumer.pic.o: consumer.cc
producer.pic.o: producer.cc
message.pic.o: message.cc
client.o: client.cc

channel_ccm.cc channel_ccm.h: channel.idl
	$(CCMGEN) $(IDLFLAGS) $^

consumer_ccm.cc consumer_ccm.h: consumer.idl
	$(CCMGEN) $^

producer_ccm.cc producer_ccm.h: producer.idl
	$(CCMGEN) $(IDLFLAGS) $^

channel.cc channel.h: channel.idl
	$(IDL) $(IDLFLAGS) $^

consumer.cc consumer.h: consumer.idl
	$(IDL) $(IDLFLAGS) $^

producer.cc producer.h: producer.idl
	$(IDL) $(IDLFLAGS) $^

message.cc message.h: message.idl
	$(IDL) $(IDLFLAGS) $^
