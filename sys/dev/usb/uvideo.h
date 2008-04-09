/*
 * Copyright (c) 2007 Robert Nagy <robert@openbsd.org>
 * Copyright (c) 2008 Marcus Glocker <mglocker@openbsd.org>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <sys/videoio.h>

/*
 * USB Video Class
 */
/* Table A-7: Video Class-Specific Endpoint Descriptor Subtypes */
#define	EP_UNDEFINED					0x00
#define EP_GENERAL					0x01
#define EP_ENDPOINT					0x02
#define EP_INTERRUPT					0x03

/* Table A-5: Video Class-Specific VC Interface Descriptor Subtypes */
#define	UDESCSUB_VC_DESCRIPTOR_UNDEFINED		0x00
#define UDESCSUB_VC_HEADER				0x01
#define UDESCSUB_VC_INPUT_TERMINAL			0x02
#define UDESCSUB_VC_OUTPUT_TERMINAL			0x03
#define UDESCSUB_VC_SELECTOR_UNIT			0x04
#define UDESCSUB_VC_PROCESSING_UNIT			0x05
#define UDESCSUB_VC_EXTENSION_UNIT			0x06

/* Table A-6: Video Class-Specific VS Interface Descriptor Subtypes */
#define	UDESCSUB_VS_UNDEFINED				0x00
#define UDESCSUB_VS_INPUT_HEADER			0x01
#define UDESCSUB_VS_OUTPUT_HEADER			0x02
#define UDESCSUB_VS_STILL_IMAGE_FRAME			0x03
#define UDESCSUB_VS_FORMAT_UNCOMPRESSED			0x04
#define UDESCSUB_VS_FRAME_UNCOMPRESSED			0x05
#define UDESCSUB_VS_FORMAT_MJPEG			0x06
#define UDESCSUB_VS_FRAME_MJPEG				0x07
#define UDESCSUB_VS_FORMAT_MPEG2TS			0x0a
#define UDESCSUB_VS_FORMAT_DV				0x0c
#define UDESCSUB_VS_COLORFORMAT				0x0d
#define UDESCSUB_VS_FORMAT_FRAME_BASED			0x10
#define UDESCSUB_VS_FRAME_FRAME_BASED			0x11
#define UDESCSUB_VS_FORMAT_STREAM_BASED			0x12

/* Table A-8: Video Class-Specific Request Codes */
#define RC_UNDEFINED					0x00
#define SET_CUR						0x01
#define GET_CUR						0x81
#define GET_MIN						0x82
#define GET_MAX						0x83
#define GET_RES						0x84
#define GET_LEN						0x85
#define GET_INFO					0x86
#define GET_DEF						0x87

/* Table A-9: Video Control Interface Control Selectors */
#define VC_CONTROL_UNDEFINED				0x00
#define VC_VIDEO_POWER_MODE_CONTROL			0x01
#define VC_REQUEST_ERROR_CODE_CONTROL			0x02

/* Table A-11: Selector Unit Control Selectors */
#define	SU_CONTROL_UNDEFINED				0x00
#define	SU_INPUT_SELECT_CONTROL 			0x01

/* Table A-12: Camera Terminal Control Selectors */
#define	CT_CONTROL_UNDEFINED				0x00
#define	CT_SCANNING_MODE_CONTROL			0x01
#define	CT_AE_MODE_CONTROL				0x02
#define	CT_AE_PRIORITY_CONTROL				0x03
#define	CT_EXPOSURE_TIME_ABSOLUTE_CONTROL		0x04
#define	CT_EXPOSURE_TIME_RELATIVE_CONTROL		0x05
#define	CT_FOCUS_ABSOLUTE_CONTROL			0x06
#define	CT_FOCUS_RELATIVE_CONTROL			0x07
#define	CT_FOCUS_AUTO_CONTROL				0x08
#define	CT_IRIS_ABSOLUTE_CONTROL			0x09
#define	CT_IRIS_RELATIVE_CONTROL			0x0a
#define	CT_ZOOM_ABSOLUTE_CONTROL			0x0b
#define	CT_ZOOM_RELATIVE_CONTROL			0x0c
#define	CT_PANTILT_ABSOLUTE_CONTROL			0x0d
#define	CT_PANTILT_RELATIVE_CONTROL			0x0e
#define	CT_ROLL_ABSOLUTE_CONTROL			0x0f
#define	CT_ROLL_RELATIVE_CONTROL			0x10
#define	CT_PRIVACY_CONTROL				0x11

/* Table A-13: Processing Unit Control Selectors */
#define	PU_CONTROL_UNDEFINED				0x00
#define	PU_BACKLIGHT_COMPENSATION_CONTROL		0x01
#define	PU_BRIGHTNESS_CONTROL				0x02
#define	PU_CONTRAST_CONTROL				0x03
#define	PU_GAIN_CONTROL 				0x04
#define	PU_POWER_LINE_FREQUENCY_CONTROL 		0x05
#define	PU_HUE_CONTROL					0x06
#define	PU_SATURATION_CONTROL				0x07
#define	PU_SHARPNESS_CONTROL				0x08
#define	PU_GAMMA_CONTROL				0x09
#define	PU_WHITE_BALANCE_TEMPERATURE_CONTROL		0x0a
#define	PU_WHITE_BALANCE_TEMPERATURE_AUTO_CONTROL	0x0b
#define	PU_WHITE_BALANCE_COMPONENT_CONTROL		0x0c
#define	PU_WHITE_BALANCE_COMPONENT_AUTO_CONTROL 	0x0d
#define	PU_DIGITAL_MULTIPLIER_CONTROL			0x0e
#define	PU_DIGITAL_MULTIPLIER_LIMIT_CONTROL		0x0f
#define	PU_HUE_AUTO_CONTROL				0x10
#define	PU_ANALOG_VIDEO_STANDARD_CONTROL		0x11
#define	PU_ANALOG_LOCK_STATUS_CONTROL			0x12

/* Table A-15: VideoStreaming Interface Control Selectors */
#define	VS_CONTROL_UNDEFINED				0x00
#define	VS_PROBE_CONTROL				0x01
#define	VS_COMMIT_CONTROL				0x02
#define	VS_STILL_PROBE_CONTROL				0x03
#define	VS_STILL_COMMIT_CONTROL 			0x04
#define	VS_STILL_IMAGE_TRIGGER_CONTROL			0x05
#define	VS_STREAM_ERROR_CODE_CONTROL			0x06
#define	VS_GENERATE_KEY_FRAME_CONTROL			0x07
#define	VS_UPDATE_FRAME_SEGMENT_CONTROL			0x08
#define	VS_SYNC_DELAY_CONTROL				0x09

/* probe commit bmRequests */
#define	UVIDEO_SET_IF					0x21
#define	UVIDEO_GET_IF					0xa1
#define	UVIDEO_SET_EP					0x22
#define	UVIDEO_GET_EP					0xa2

/* Table B-1: USB Terminal Types */
#define	TT_VENDOR_SPECIFIC				0x0100
#define	TT_STREAMING					0x0101

/* Table B-2: Input Terminal Types */
#define	ITT_VENDOR_SPECIFIC				0x0200
#define	ITT_CAMERA					0x0201
#define	ITT_MEDIA_TRANSPORT_INPUT			0x0202

/* Table B-3: Output Terminal Types */
#define	OTT_VENDOR_SPECIFIC				0x0300
#define	OTT_DISPLAY					0x0301
#define	OTT_MEDIA_TRANSPORT_OUTPUT			0x0302

/* Table B-4: External Terminal Types */
#define	EXTERNAL_VENDOR_SPECIFIC			0x0400
#define	COMPOSITE_CONNECTOR				0x0401
#define	SVIDEO_CONNECTOR				0x0402
#define	COMPONENT_CONNECTOR				0x0403

/* Table 3-3: VC Interface Header Descriptor */
struct usb_video_header_descriptor {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uWord	bcdUVC;
	uWord	wTotalLength;
	uDWord	dwClockFrequency; /* XXX deprecated */
	uByte	bInCollection;
};

struct usb_video_control {
	struct usb_video_header_descriptor	*descr;
	uByte					*baInterfaceNr;
};

struct usb_video_header_desc {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uWord	bcdUVC;
	uWord	wTotalLength;
	uDWord	dwClockFrequency; /* XXX deprecated */
	uByte	bInCollection;
	uByte	baInterfaceNr;
};

/* Table 3-4: Input Terminal Descriptor */
struct usb_video_input_terminal_desc {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uByte	bTerminalID;
	uWord	wTerminalType;
	uByte	bAssocTerminal;
	uByte	iTerminal;
};

/* Table 3-5: Output Terminal Descriptor */
struct usb_video_output_terminal_desc {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uByte	bTerminalID;
	uWord	wTerminalType;
	uByte	bAssocTerminal;
	uByte	bSourceID;
	uByte	iTerminal;
};

/* Table 3-6: Camera Terminal Descriptor */
struct usb_video_camera_terminal_desc {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uByte	bTerminalID;
	uWord	wTerminalType;
	uByte	bAssocTerminal;
	uByte	iTerminal;
	uWord	wObjectiveFocalLengthMin;
	uWord	wObjectiveFocalLengthMax;
	uWord	wOcularFocalLength;
	uByte	bControlSize;
	uByte	*bmControls; /* XXX */	
};

/* Table 3-11: VC Endpoint Descriptor */
struct usb_video_vc_endpoint_desc {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uWord	wMaxTransferSize;
};

/* Table 3-18: Color Matching Descriptor */
struct usb_video_color_matching_descr {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uByte	bColorPrimaries;
	uByte	bTransferCharacteristics;
	uByte	bMatrixCoefficients;
} __packed; 

/* Table 4-47: Video Probe and Commit Controls */
struct usb_video_probe_commit {
	uByte	bmHint[2];
	uByte	bFormatIndex;
	uByte	bFrameIndex;
	uByte	dwFrameInterval[4];
	uByte	wKeyFrameRate[2];
	uByte	wPFrameRate[2];
	uByte	wCompQuality[2];
	uByte	wCompWindowSize[2];
	uByte	wDelay[2];
	uByte	dwMaxVideoFrameSize[4];
	uByte	dwMaxPayloadTransferSize[4];
	uByte	wClockFrequency[4];
	uByte	bmFramingInfo;
	uByte	bPreferedVersion;
	uByte	bMinVersion;
	uByte	bMaxVersion;
} __packed;

/*
 * USB Video Payload Uncompressed
 */
/* Table 2-1: Compression Formats */
#define	UVIDEO_FORMAT_GUID_YUY2	{			\
    0x59, 0x55, 0x59, 0x32, 0x00, 0x00, 0x10, 0x00,	\
    0x80, 0x00, 0x00, 0xaa, 0x00, 0x38, 0x9b, 0x71 }

#define	UVIDEO_FORMAT_GUID_NV12	{			\
    0x4e, 0x56, 0x31, 0x32, 0x00, 0x00, 0x10, 0x00,	\
    0x80, 0x00, 0x00, 0xaa, 0x00, 0x38,	0x9b, 0x71 }

/*
 * USB Video Payload MJPEG
 */
/* Table 2-1: Stream Header Format for the Motion-JPEG */
#define	UVIDEO_STREAM_FID	(1 << 0)
#define	UVIDEO_STREAM_EOF	(1 << 1)
#define	UVIDEO_STREAM_PTS	(1 << 2)
#define	UVIDEO_STREAM_SCR	(1 << 3)
#define	UVIDEO_STREAM_RES	(1 << 4)
#define	UVIDEO_STREAM_STI	(1 << 5)
#define	UVIDEO_STREAM_ERR	(1 << 6)
#define	UVIDEO_STREAM_EOH	(1 << 7)

/* Table 3-1: Motion-JPEG Video Format Descriptor */
struct usb_video_format_mjpeg_descriptor {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uByte	bFormatIndex;
	uByte	bNumFrameDescriptors;
	uByte	bmFlags;
	uByte	bDefaultFrameIndex;
	uByte	bAspectRatioX;
	uByte	bAspectRatioY;
	uByte	bmInterlaceFlags;
	uByte	bCopyProtect;
} __packed;

/* Table 3-2: Motion-JPEG Video Frame Descriptor */
struct usb_video_frame_descriptor {
	uByte	bLength;
	uByte	bDescriptorType;
	uByte	bDescriptorSubtype;
	uByte	bFrameIndex;
	uByte	bmCapabilities;
	uWord	wWidth;
	uWord	wHeight;
	uDWord	dwMinBitRate;
	uDWord	dwMaxBitRate;
	uDWord	dwMaxVideoFrameBufferSize;
	uWord	dwDefaultFrameInterval;
	uByte	bFrameIntervalType;
} __packed;

/*
 * Driver specific private definitions.
 */
#define UVIDEO_NFRAMES		10 /* XXX calculate right value */

struct uvideo_stream_if {
	struct uvideo_softc	*sc;
	usbd_xfer_handle	 xfer;
	void			*buf;
	int			 busy;
	int			 numalts;
	usbd_interface_handle  	 in_ifaceh;
	usbd_pipe_handle	 in_pipeh;
	int			 endpoint;
	u_int16_t		 size[UVIDEO_NFRAMES];

	u_int8_t		 fmtgrp_cnt;

	usbd_interface_handle	 ifaceh;
	usbd_interface_handle	*if_descr;
	int			 curr_alt;
	u_int32_t		 max_isoc_payload;

	char			 start_polling;
	char			 fid;
};

struct uvideo_sample_buffer {
	int		 fragment;
	uint8_t		 fid;
	int		 offset;
	uint8_t		*buf;
};

struct uvideo_softc {
	struct device				 sc_dev;
	usbd_device_handle			 sc_udev;
	usbd_interface_handle			 sc_iface;
	int					 sc_iface_number;
	int					 sc_product;
	int					 sc_vendor;

	int					 sc_intr_number;
	usbd_pipe_handle			 sc_intr_pipe;
	u_char					*sc_ibuf;
	int					 sc_isize;
	int					 sc_vc_iface;

	struct device				*sc_videodev;

	struct vs_info				*sc_alts;
	int					 sc_nalts;
	int					 sc_nullalt;
	int					 sc_video_rev;
	int					 sc_enabled;
	int					 sc_dying;
	int					 sc_mode;

	u_int16_t				 uvc_version;
	u_int32_t				 clock_frequency;
	u_int32_t				 quirks;

	struct uvideo_sample_buffer		 sc_sample_buffer;

	struct vnode				*sc_vp;
	struct usb_task				 sc_task_write;

	struct usb_video_control		*sc_vc_header;
	struct uvideo_stream_if			 sc_curr_strm;
};
