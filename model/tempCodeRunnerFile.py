    if nose_x and nose_y:
                cv2.circle(image, (int(nose_x), int(nose_y)), 7, pink, -1)
            if l_shldr_x and l_shldr_y:
                cv2.circle(image, (int(l_shldr_x), int(l_shldr_y)), 7, red, -1)
            if r_shldr_x and r_shldr_y:
                cv2.circle(image, (int(r_shldr_x), int(r_shldr_y)), 7, red, -1)

            # Draw lines between shoulders and nose (assuming you have detection logic)
            if l_shldr_x and l_shldr_y and r_shldr_x and r_shldr_y and nose_x and nose_y:
                cv2.line(image, (int(l_shldr_x), int(l_shldr_y)), (int(r_shldr_x), int(r_shldr_y)), green, 2)
                cv2.line(image, (int(l_shldr_x), int(l_shldr_y)), (int(nose_x), int(nose_y)), green, 2)
                cv2.line(image, (int(r_shldr_x), int(r_shldr_y)), (int(nose_x), int(nose_y)), green, 2)